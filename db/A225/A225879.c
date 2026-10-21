/*
 * A225879  Number of n-length words w over ternary alphabet {1,2,3} such that
 *          for every prefix z of w we have 0<=#(z,1)-#(z,2)<=2 and
 *          0<=#(z,2)-#(z,3)<=2 and #(z,x) gives the number of occurrences of
 *          letter x in z.
 * https://oeis.org/A225879
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A225879 */
#include <stdlib.h>
#include <stdbool.h>

/*
   Sequence definition:
   a(0) = 1
   a(1) = 1
   a(2) = 2
   a(3) = 3
   a(4) = 7
   a(5) = 14
   a(6) = 23
   ...
   The recurrence used: a(n) = 7*a(n-3) + 2*a(n-6) for n>5.
   The implementation below uses a direct linear‑time DP that counts
   admissible words of each length using a 3×3 state machine
   (differences between #1 and #2, and between #2 and #3 are kept
   in the range [0,2]).  Results are memoised in a static cache
   so that arbitrary call order is handled efficiently.
*/

seq_t a(idx_t n)
{
    /* static cache – memoisation of previously computed terms */
    static bool initialized = false;
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cache_max = 0;  /* highest n for which cache holds data */

    /* state arrays for the DP (3×3 grid flattened) */
    static seq_t cur[9];
    static seq_t nxt[9];

    /* initialise once */
    if (!initialized) {
        /* All 9 states start with value 0, except the initial (0,0) state. */
        for (int i = 0; i < 9; ++i) cur[i] = 0;
        cur[0] = 1;                      /* (d12=0, d23=0) */
        cache_cap = 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = 1;                    /* a(0) = 1 */
        cache_max = 0;
        initialized = true;
    }

    /* enlarge cache if required */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        cache_cap = new_cap;
    }

    /* if already computed, return cached value */
    if (n <= cache_max) return cache[(size_t)n];

    /* compute from cache_max+1 up to n */
    for (idx_t len = cache_max + 1; len <= n; ++len) {
        /* zero the next‑state array */
        for (int k = 0; k < 9; ++k) nxt[k] = 0;

        /* iterate over all 9 states (d12,d23) */
        for (int d12 = 0; d12 <= 2; ++d12) {
            for (int d23 = 0; d23 <= 2; ++d23) {
                seq_t val = cur[3 * d12 + d23];
                if (val == 0) continue;

                /* append letter 1: d12 increases, d23 unchanged */
                if (d12 < 2)
                    nxt[3 * (d12 + 1) + d23] += val;

                /* append letter 2: d12 decreases, d23 increases */
                if (d12 > 0 && d23 < 2)
                    nxt[3 * (d12 - 1) + (d23 + 1)] += val;

                /* append letter 3: d12 unchanged, d23 decreases */
                if (d23 > 0)
                    nxt[3 * d12 + (d23 - 1)] += val;
            }
        }

        /* copy next to current and compute sum = a(len) */
        seq_t sum = 0;
        for (int k = 0; k < 9; ++k) {
            cur[k] = nxt[k];
            sum += cur[k];
        }
        cache[(size_t)len] = sum;
    }

    cache_max = n;
    return cache[(size_t)n];
}
