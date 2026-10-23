/*
 * A156762  Denominator of Euler(n, 1/21).
 * https://oeis.org/A156762
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 21^n * 2^v2(n+1), where v2(k) is the exponent of 2 in k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline idx_t v2(idx_t x)
{
    idx_t c = 0;
    while ((x & 1) == 0) {
        ++c;
        x >>= 1;
    }
    return c;
}

seq_t a(idx_t n)
{
    static seq_t *cache       = NULL;
    static idx_t cache_len    = 0;     /* number of elements stored in cache   */
    static seq_t pow21 = (seq_t)1;    /* 21^cache_len                         */

    if (n < cache_len)
        return cache[n];

    /* Reallocate cache to hold up to n */
    size_t needed = (size_t)n + 1;
    cache = (seq_t*)realloc(cache, needed * sizeof(seq_t));
    if (!cache)
        return 0; /* out of memory */

    for (idx_t i = cache_len; i <= n; ++i) {
        /* pow21 currently holds 21^i            */
        idx_t exp = v2((idx_t)(i + 1));
        seq_t factor = ((seq_t)1) << exp;       /* 2^exp */
        cache[i] = pow21 * factor;
        pow21 *= (seq_t)21;                     /* prepare 21^(i+1) */
    }
    cache_len = needed;
    return cache[n];
}
