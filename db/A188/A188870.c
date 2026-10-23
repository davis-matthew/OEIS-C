/*
 * A188870  Number of n X 5 binary arrays without the pattern 0 0 0
 *          antidiagonally or horizontally.
 * https://oeis.org/A188870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A188870: Number of n×5 binary arrays with no 0‑0‑0 horizontally or antidiagonally.
   Recurrence (n > 12):
   a(n) = 24*a(n-1) - 67*a(n-2) - 48*a(n-3) - 540*a(n-4) + 2272*a(n-5) + 1536*a(n-6)
          - 6944*a(n-7) + 2864*a(n-8) + 1600*a(n-9) - 768*a(n-10)
   Initial values: a(1)=24, a(2)=576, a(3)=11332, a(4)=233756, a(5)=4805140,
   a(6)=98892196, a(7)=2035428944, a(8)=41894114820, a(9)=862288002496,
   a(10)=17748103310980, a(11)=365301608074080, a(12)=7518846739068100.
*/

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* ---------- static state ---------- */
    static bool initialized = false;
    static seq_t *cache = NULL;          /* array of computed terms, a[1] -> cache[0] */
    static size_t cache_len = 0;          /* number of terms stored */
    static size_t cache_cap = 0;          /* allocated capacity */

    /* ---------- constants ---------- */
    enum { ROWS = 32, STATES = 1024 };

    /* ---------- internal helpers ---------- */
    static bool *valid = NULL;                   /* valid[row] == true if row contains no 000 horizontally */
    static unsigned int *trans_mask = NULL;      /* bitmask of allowed next rows for each state */
    static seq_t *dp_cur = NULL;
    static seq_t *dp_next = NULL;

    if (!initialized) {
        /* ---- compute valid rows ---- */
        valid = (bool*)malloc(ROWS * sizeof(bool));
        for (int r = 0; r < ROWS; ++r) {
            bool ok = true;
            for (int c = 0; c <= 2; ++c) {
                unsigned int bits = (r >> c) & 7; /* 3 bits starting at c */
                if ((bits & 1) == 0 && (bits & 2) == 0 && (bits & 4) == 0) {
                    ok = false;
                    break;
                }
            }
            valid[r] = ok;
        }

        /* ---- precompute transition masks ---- */
        trans_mask = (unsigned int*)malloc(STATES * sizeof(unsigned int));
        for (int p1 = 0; p1 < ROWS; ++p1) {
            for (int p2 = 0; p2 < ROWS; ++p2) {
                unsigned int mask = 0;
                if (valid[p1] && valid[p2]) {
                    for (int nrow = 0; nrow < ROWS; ++nrow) {
                        if (!valid[nrow]) continue;
                        bool bad = false;
                        for (int c = 0; c <= 2; ++c) {
                            bool v1 = ((p1 >> c) & 1) == 0;
                            bool v2 = ((p2 >> (c + 1)) & 1) == 0;
                            bool v3 = ((nrow >> (c + 2)) & 1) == 0;
                            if (v1 && v2 && v3) {
                                bad = true;
                                break;
                            }
                        }
                        if (!bad) mask |= 1u << nrow;
                    }
                }
                trans_mask[p1 * ROWS + p2] = mask;
            }
        }

        /* ---- dp arrays ---- */
        dp_cur  = (seq_t*)calloc(STATES, sizeof(seq_t));
        dp_next = (seq_t*)calloc(STATES, sizeof(seq_t));

        /* ---- compute first 12 terms via DP ---- */
        seq_t first12[12];

        /* a(1) */
        seq_t cnt_rows = 0;
        for (int r = 0; r < ROWS; ++r) {
            if (valid[r]) cnt_rows += 1;
        }
        first12[0] = cnt_rows;

        /* a(2) */
        for (int r1 = 0; r1 < ROWS; ++r1) {
            if (!valid[r1]) continue;
            for (int r2 = 0; r2 < ROWS; ++r2) {
                if (!valid[r2]) continue;
                dp_cur[(r1 << 5) | r2] = 1;
            }
        }
        seq_t sum2 = 0;
        for (int st = 0; st < STATES; ++st) sum2 += dp_cur[st];
        first12[1] = sum2;

        /* a(3)..a(12) */
        for (int step = 3; step <= 12; ++step) {
            /* zero dp_next */
            for (int st = 0; st < STATES; ++st) dp_next[st] = 0;
            /* transition */
            for (int st = 0; st < STATES; ++st) {
                seq_t val = dp_cur[st];
                if (!val) continue;
                unsigned int mask = trans_mask[st];
                if (!mask) continue;
                int prev1 = st >> 5;
                int prev2 = st & 31;
                for (int nrow = 0; nrow < ROWS; ++nrow) {
                    if (mask & (1u << nrow)) {
                        int nxt = (prev2 << 5) | nrow;
                        dp_next[nxt] += val;
                    }
                }
            }
            /* compute total for this step */
            seq_t total = 0;
            for (int st = 0; st < STATES; ++st) total += dp_next[st];
            first12[step - 1] = total;
            /* swap */
            seq_t *tmp = dp_cur;
            dp_cur = dp_next;
            dp_next = tmp;
        }

        /* cache first 12 terms */
        cache_cap = 12;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        for (int i = 0; i < 12; ++i) cache[i] = first12[i];
        cache_len = 12;

        initialized = true;
    }

    /* ---------- possibly extend cache ---------- */
    if ((idx_t)cache_len >= n) {
        return cache[(size_t)(n - 1)];
    }

    /* enlarge allocation if necessary */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : n;
        if (new_cap < (size_t)n) new_cap = n;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* ---------- compute missing terms ---------- */
    for (size_t k = cache_len + 1; k <= (size_t)n; ++k) {
        /* recurrence for k > 12 */
        seq_t sum = 24 * cache[k - 2];
        sum -= 67 * cache[k - 3];
        sum -= 48 * cache[k - 4];
        sum -= 540 * cache[k - 5];
        sum += 2272 * cache[k - 6];
        sum += 1536 * cache[k - 7];
        sum -= 6944 * cache[k - 8];
        sum += 2864 * cache[k - 9];
        sum += 1600 * cache[k - 10];
        sum -= 768 * cache[k - 11];
        cache[k - 1] = sum;
    }
    cache_len = n;

    return cache[(size_t)(n - 1)];
}
