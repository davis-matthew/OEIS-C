/*
 * A229515  Number of n X 2 0..2 arrays of the median of the corresponding
 *          element, the element to the east and the element to the south in a
 *          larger (n+1) X 3 0..2 array without adjacent equal elements in the
 *          latter.
 * https://oeis.org/A229515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n X 2 0..2 arrays of the median of the
 *            corresponding element, the element to the east and the element
 *            to the south in a larger (n+1) X 3 0..2 array without adjacent
 *            equal elements in the latter
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/* number of valid 3‑color rows with horizontal adjacency constraint   */
#define ROWS          12
/* bit mask of all rows                                         */
#define ALL_ROWS      ((1u << ROWS) - 1)
/* number of possible output letters (two digits base‑3)         */
#define LETTERS       9

/* static helpers ------------------------------------------------- */
static void build_rows(int rows[ROWS][3]) {
    int k = 0;
    for (int a = 0; a < 3; ++a)
        for (int b = 0; b < 3; ++b)
            for (int c = 0; c < 3; ++c)
                if (a != b && b != c) {
                    rows[k][0] = a; rows[k][1] = b; rows[k][2] = c;
                    ++k;
                }
}

static int median3(int x, int y, int z) {
    int mn = x < y ? x : y;  if (z < mn) mn = z;
    int mx = x > y ? x : y;  if (z > mx) mx = z;
    return x + y + z - mn - mx;
}

/* main sequence function ---------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;   /* outside offset, not used */

    /* static data ----------------------------------------------- */
    static int rows[ROWS][3];
    static uint16_t edge_mask[ROWS][LETTERS];     /* transition masks for each state & letter */
    static uint16_t trans_next[ALL_ROWS + 1][LETTERS];
    static uint8_t  trans_cnt[ALL_ROWS + 1][LETTERS];
    static uint8_t  trans_len[ALL_ROWS + 1];
    static seq_t   dp_cur[ALL_ROWS + 1];
    static seq_t   dp_next[ALL_ROWS + 1];
    static seq_t  *cache = NULL;
    static idx_t   cache_size = 0;
    static idx_t   computed_up_to = 0;
    static uint8_t initialized = 0;

    /* initialise static tables once -------------------------------- */
    if (!initialized) {
        /* build all valid rows */
        build_rows(rows);

        /* build edge mask for each state & letter (bitmask of target rows) */
        for (int s = 0; s < ROWS; ++s)
            for (int le = 0; le < LETTERS; ++le)
                edge_mask[s][le] = 0;

        for (int s = 0; s < ROWS; ++s)
            for (int t = 0; t < ROWS; ++t) {
                /* vertical adjacency constraint */
                if (rows[s][0] == rows[t][0] ||
                    rows[s][1] == rows[t][1] ||
                    rows[s][2] == rows[t][2]) continue;
                int a = median3(rows[s][0], rows[t][0], rows[s][1]);
                int b = median3(rows[s][1], rows[t][1], rows[s][2]);
                int letter = a * 3 + b;          /* 0..8 */
                edge_mask[s][letter] |= (1u << t);
            }

        /* build transitions for every subset mask */
        for (int mask = 0; mask <= ALL_ROWS; ++mask) {
            trans_len[mask] = 0;
            if (!mask) continue;
            int cnt[ALL_ROWS + 1];
            memset(cnt, 0, sizeof(cnt));
            for (int le = 0; le < LETTERS; ++le) {
                uint16_t m = 0;
                for (int s = 0; s < ROWS; ++s)
                    if (mask & (1u << s))
                        m |= edge_mask[s][le];
                if (m) cnt[m] += 1;
            }
            for (int nm = 1; nm <= ALL_ROWS; ++nm)
                if (cnt[nm]) {
                    uint8_t i = trans_len[mask];
                    trans_next[mask][i] = nm;
                    trans_cnt[mask][i] = (uint8_t)cnt[nm];
                    trans_len[mask] = i + 1;
                }
        }

        /* initial DP state: word of length 0 */
        memset(dp_cur, 0, sizeof(dp_cur));
        dp_cur[ALL_ROWS] = 1;   /* start mask with all 12 rows set */

        /* cache for results */
        cache = NULL;
        cache_size = 0;
        computed_up_to = 0;

        initialized = 1;
    }

    /* extend cache if necessary ---------------------------------- */
    if (cache == NULL || cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure */
        cache = tmp;
        cache_size = new_size;
    }

    /* compute missing terms -------------------------------------- */
    for ( ; computed_up_to < n; ++computed_up_to) {
        memset(dp_next, 0, sizeof(dp_next));
        for (int mask = 1; mask <= ALL_ROWS; ++mask) {
            seq_t val = dp_cur[mask];
            if (val == 0) continue;
            for (int i = 0; i < trans_len[mask]; ++i) {
                uint16_t nm = trans_next[mask][i];
                uint8_t  cnt = trans_cnt[mask][i];
                dp_next[nm] += val * (seq_t)cnt;
            }
        }
        /* total number of distinct words of this length */
        seq_t total = 0;
        for (int mask = 1; mask <= ALL_ROWS; ++mask)
            total += dp_next[mask];
        cache[computed_up_to + 1] = total;
        memcpy(dp_cur, dp_next, sizeof(dp_cur));
    }

    return cache[n];
}
