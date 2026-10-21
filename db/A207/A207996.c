/*
 * A207996  Number of nX7 0..2 arrays with new values 0..2 introduced in row
 *          major order and no element equal to any horizontal or vertical
 *          neighbor (colorings ignoring permutations of colors).
 * https://oeis.org/A207996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A207996 – Number of n×7 0..2 arrays with new values 0..2 introduced in
    row‑major order and no element equal to any horizontal or vertical
    neighbor.  The values count colorings up to color permutations.
    The sequence satisfies an order‑13 linear recurrence for n ≥ 14;
    the first 13 terms are obtained by a transfer‑matrix DP.
    Initial values (n = 1..13):
        32, 729, 17749, 441090, 11045757, 277458045, 6978332618,
        175605187731, 4419979346851, 111261280858024,
        2800819361992659, 70507189155056781, 1774944424628356230
    Recurrence for n ≥ 14:
        a(n)=55*a(n-1)-1109*a(n-2)+11330*a(n-3)-67206*a(n-4)
             +247404*a(n-5)-582440*a(n-6)+881876*a(n-7)
             -846764*a(n-8)+499200*a(n-9)-172400*a(n-10)
             +33152*a(n-11)-3264*a(n-12)+128*a(n-13) */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return the n-th term of the sequence.  */
seq_t a(idx_t n)
{
    /* Static cache for computed terms. */
    static seq_t *cache   = NULL;   /* cache[i] = a(i), 1‑based */
    static size_t cache_cap = 0;    /* allocated size (≥ cache_len+1) */
    static idx_t  cache_len = 0;    /* highest n computed so far */

    /* Do not compute for n < 1. */
    if (n < 1) return 0;

    /* First call: pre‑compute the first 13 terms by DP. */
    if (cache == NULL) {
        /* 3^7 = 2187 possible colorings; only 192 have no equal horizontal neighbours. */
        const int ROWS   = 7;
        const int TOTAL  = 2187;          /* 3^7 */
        const int MAX_VALID = 192;

        /* Allocate space for all states (stateColors[s][r]) */
        int *data = (int *)malloc(TOTAL * ROWS * sizeof(int));
        if (!data) return 0; /* allocation failure */

        int (*stateColors)[ROWS] = (int (*)[ROWS])data;
        int stateCnt = 0;

        for (int s = 0; s < TOTAL; ++s) {
            int tmp = s;
            bool ok = true;
            int prev = -1;
            for (int r = 0; r < ROWS; ++r) {
                int col = tmp % 3;
                tmp   /= 3;
                stateColors[s][r] = col;
                if (col == prev) { ok = false; break; }
                prev = col;
            }
            if (ok) {
                /* keep only valid states */
                for (int r = 0; r < ROWS; ++r)
                    stateColors[stateCnt][r] = stateColors[s][r];
                ++stateCnt;
            }
        }
        /* stateCnt should be MAX_VALID (192). */

        /* Dynamic programming for n columns */
        size_t sc = (size_t)stateCnt;
        seq_t *dp_prev = (seq_t *)malloc(sc * sizeof(seq_t));
        seq_t *dp_next = (seq_t *)malloc(sc * sizeof(seq_t));
        if (!dp_prev || !dp_next) { free(data); return 0; }

        /* first column: each state occurs once */
        for (size_t i = 0; i < sc; ++i) dp_prev[i] = 1;
        /* total colorings for the first column */
        seq_t total = 0;
        for (size_t i = 0; i < sc; ++i) total += dp_prev[i];

        /* Initialize cache up to term 13 */
        cache_cap = 14;          /* indices 0..13 */
        cache_len = 13;
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { free(data); free(dp_prev); free(dp_next); return 0; }
        cache[1] = total / 6;    /* a(1) */

        /* compute a(2) … a(13) */
        for (idx_t col = 2; col <= 13; ++col) {
            for (size_t i = 0; i < sc; ++i) {
                seq_t sum = 0;
                for (size_t j = 0; j < sc; ++j) {
                    bool ok = true;
                    for (int r = 0; r < ROWS; ++r) {
                        if (stateColors[i][r] == stateColors[j][r]) { ok = false; break; }
                    }
                    if (ok) sum += dp_prev[j];
                }
                dp_next[i] = sum;
            }
            total = 0;
            for (size_t i = 0; i < sc; ++i) total += dp_next[i];
            cache[col] = total / 6;   /* a(col) */

            /* swap dp arrays */
            seq_t *tmp = dp_prev; dp_prev = dp_next; dp_next = tmp;
        }

        free(dp_prev);
        free(dp_next);
        /* keep stateColors data allocated for future calls */
    }

    /* Extend cache if a larger term is requested. */
    if (n > cache_len) {
        /* Ensure capacity. */
        if ((size_t)n + 1 > cache_cap) {
            size_t new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0; /* allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Compute terms using the recurrence. */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = 0;
            val += cache[i-1] * 55;
            val -= cache[i-2] * 1109;
            val += cache[i-3] * 11330;
            val -= cache[i-4] * 67206;
            val += cache[i-5] * 247404;
            val -= cache[i-6] * 582440;
            val += cache[i-7] * 881876;
            val -= cache[i-8] * 846764;
            val += cache[i-9] * 499200;
            val -= cache[i-10] * 172400;
            val += cache[i-11] * 33152;
            val -= cache[i-12] * 3264;
            val += cache[i-13] * 128;
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
