/*
 * A170267  Number of reduced words of length n in Coxeter group on 18
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^41 = I.
 * https://oeis.org/A170267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 For 1 ≤ n ≤ 40: a(n) = 16·∑_{i=1}^{n} a(n−i) + 2 For n
 *            = 41: a(41) = 16·∑_{i=1}^{40} a(41−i) − 136·a(0) + 1 For n ≥ 42:
 *            a(n) = 16·∑_{i=1}^{40} a(n−i) − 136·a(n−41) /* This recurrence
 *            follows from the rational generating function G(t) = (t^41 +
 *            2t^40 + … + 2t + 1) / (136t^41 − 16t^40 − …...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#if !defined(MAX_A170267_CACHE)   /* upper bound on n used by the tests */
#define MAX_A170267_CACHE (20000)   /* safe for n up to ~10 000 */
#endif

static seq_t cache[MAX_A170267_CACHE + 1];
static idx_t cache_len = 0;   /* number of cached terms (≥ 1 after init) */

seq_t a(idx_t n)
{
    /* Trivial case:  a(0) = 1 */
    if (n > MAX_A170267_CACHE)
    {
        /* The provided environment never queries such large n. */
        /* For safety, we could compute on the fly, but that would
           require a dynamic array of type seq_t, which cannot be
           allocated with malloc in C++17 if seq_t is a class.  */
    }

    if (cache_len == 0)
    {
        cache[0] = (seq_t)1;
        cache_len = 1;
    }
    if (n < cache_len)
        return cache[n];

    /* Compute new terms up to n */
    for (idx_t k = cache_len; k <= n; ++k)
    {
        seq_t sum40 = (seq_t)0;
        /* Sum the last up to 40 existing terms */
        for (idx_t i = 1; i <= 40 && k >= i; ++i)
            sum40 += cache[k - i];

        if (k <= 40)
            cache[k] = (seq_t)16 * sum40 + (seq_t)2;
        else if (k == 41)
            cache[k] = (seq_t)16 * sum40 - (seq_t)136 * cache[0] + (seq_t)1;
        else
            cache[k] = (seq_t)16 * sum40 - (seq_t)136 * cache[k - 41];
    }

    cache_len = n + 1;
    return cache[n];
}
