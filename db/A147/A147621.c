/*
 * A147621  The 3rd Witt transform of A000292.
 * https://oeis.org/A147621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) is the coefficient of x^n in * G(x) = x^4*(2 - x + 2x^2)*(2
 *            - 2x + 9x^2 - 2x^3 + 2x^4) / * ((1 - x)^12 * (1 + x + x^2)^4) *
 *            Hence a(n) satisfies the linear recurrence * Σ_{k=0}^{20} Q[k] *
 *            a(n-k) = N[n], a(m)=0 (m<0) * with Q[0..20] and N[0..10] below
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 37, u64 n <= 303,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t Q[21] = {
    /* (1‑x)^12 · (1+x+x²)^4 */
    1, -8, 28, -60, 102, -168, 258, -336, 393, -452,
    484, -452, 393, -336, 258, -168, 102, -60, 28, -8, 1
};

static const seq_t N[11] = {
    /* x⁴·(2‑x+2x²)·(2‑2x+9x²‑2x³+2x⁴) */
    0, 0, 0, 0, 4, -6, 24, -17, 24, -6, 4
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure the cache holds indices up to n. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();      /* allocation failure */
        cache = tmp;

        /* Compute new terms iteratively. */
        for (size_t i = cache_cap; i < new_cap; ++i) {
            seq_t sum = 0;
            /* sum over k = 1..min(20,i) of Q[k] * a(i-k) */
            size_t k_max = (i < 20) ? i : 20;
            for (size_t k = 1; k <= k_max; ++k) {
                sum += (seq_t)Q[k] * cache[i - k];
            }
            seq_t rhs = (i < 11) ? N[i] : 0;
            cache[i] = rhs - sum;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
