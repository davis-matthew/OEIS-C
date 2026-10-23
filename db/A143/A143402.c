/*
 * A143402  Expansion of x^k/Product_{t=k..2k} (1-tx) for k=7.
 * https://oeis.org/A143402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the coefficient of x^n in x^7 / ∏_{t=7}^{14} (1 - t x).
 *            Using the relation P(x) · G(x) = x^7, where
 *            P(x)=∏(1-tx)=∑_{k=0}^8 (-1)^k e_k x^k, we obtain the recurrence
 *            for n≥8 a(n) = Σ_{k=1..8} (-1)^{k+1} e_k · a(n-k), with e_k the
 *            elementary symmetric sums of 7,…,14, and initial values a(0)=...
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 13, u64 n <= 20, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Cache for elementary symmetric sums e[0..8] of 7..14 */
    static seq_t e[9];
    static bool e_initialized = false;

    if (!e_initialized) {
        e[0] = 1;                    /* e_0 = 1 */
        for (idx_t t = 7; t <= 14; ++t) {
            /* update e[k] from high to low to avoid overwriting needed values */
            for (int k = 8; k >= 1; --k) {
                e[k] = e[k] + (seq_t)t * e[k-1];
            }
        }
        e_initialized = true;
    }

    /* Cache for sequence values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of entries currently stored in cache */

    /* Ensure cache holds indices up to n */
    if (n >= cache_len) {
        size_t needed = (size_t)n + 1;
        size_t new_cap = cache_len ? cache_len * 2 : 16;
        if (new_cap < needed) new_cap = needed;

        /* realloc cast is acceptable in both C and C++ */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (cache == NULL) return 0; /* allocation failure – return 0 as a safe fallback */

        /* Initialise base values a(0)..a(7) */
        if (cache_len == 0) {
            for (size_t i = 0; i < 7; ++i) cache[i] = 0;
            cache[7] = 1;
            cache_len = 8;
        }

        /* Compute terms from cache_len up to n */
        for (size_t i = cache_len; i <= n; ++i) {
            if (i < 7) {
                cache[i] = 0;
            } else if (i == 7) {
                cache[i] = 1;
            } else {
                seq_t sum = 0;
                for (int k = 1; k <= 8; ++k) {
                    if (i < (size_t)k) break;          /* a(i-k) undefined for i<k */
                    seq_t prod = e[k] * cache[i - k];   /* e_k * a(i-k) */
                    if (k & 1)                        /* k odd → sign +1 */
                        sum = sum + prod;
                    else                               /* k even → sign -1 */
                        sum = sum - prod;
                }
                cache[i] = sum;
            }
        }
        cache_len = needed;
    }

    return cache[(size_t)n];
}
