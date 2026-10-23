/*
 * A170666  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^49 = I.
 * https://oeis.org/A170666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence a(0) = 1, a(n) = N(n) + 31
 *            · Σ_{k=1}^{48} a(n-k) (for 1 ≤ n < 49, where N(0)=1, N(1..48)=2,
 *            N(49)=1), a(n) = 31 · Σ_{k=1}^{48} a(n-k) – 496 · a(n-49) (for n
 *            ≥ 49) where the recurrence comes from the rational generating
 *            function N(t)/(1 – 31t – 31t² – … – 31t⁴⁸...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cache_len = 0; /* number of terms stored */

static inline void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = (n + 1) * 2;
    seq_t *new_mem = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!new_mem) abort(); /* allocation failure */
    cache = new_mem;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    ensure_cache(n);
    if (cache_len > 0 && n < cache_len)
        return cache[n];
    /* Compute missing terms up to n */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t val = 0;
        /* N(n) term for 0 <= i <= 49 */
        seq_t Ncoeff = (seq_t)(
            i == 0        ? 1
          : i == 49       ? 1
          :                   2); /* i in [1..48] */

        /* Sum of last 48 terms */
        seq_t sum48 = 0;
        idx_t upto = (i >= 48) ? 48 : (idx_t)i;
        for (idx_t k = 1; k <= upto; ++k)
            sum48 += cache[i - k];

        if (i < 49) {
            /* a(i) = N(i) + 31*sum48 */
            val = Ncoeff + 31 * sum48;
        } else {
            /* a(i) = 31*sum48 - 496*a(i-49) */
            val = 31 * sum48 - 496 * cache[i - 49];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}
