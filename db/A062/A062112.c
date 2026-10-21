/*
 * A062112  a(0)=0; a(1)=1; a(n) = a(n-1) + (3 + (-1)^n)*a(n-2)/2.
 * https://oeis.org/A062112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0; a(1)=1; a(n)=a(n-1)+((n%2==0)?2:1)*a(n-2)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 37, u64 n <= 73, u128 n <= 146,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* initialise base terms */
    if (cache_len == 0)
    {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;           /* should never happen in tests */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* grow array if needed */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* already computed? */
    if (n < cache_len)
        return cache[(size_t)n];

    /* compute missing values */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        seq_t a1 = cache[(size_t)(i - 1)];
        seq_t a2 = cache[(size_t)(i - 2)];
        seq_t term = a1;
        if ((i & 1) == 0)          /* even n: coefficient 2 */
            term += ((seq_t)2) * a2;
        else                       /* odd n: coefficient 1 */
            term += a2;
        cache[(size_t)i] = term;
    }
    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
