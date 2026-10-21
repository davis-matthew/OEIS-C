/*
 * A052654  E.g.f. 1/((1-x)(1-4x)).
 * https://oeis.org/A052654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((4^(n+1)-1)/3) * n!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static caches for factorials and powers of 4.
       They are expanded only forward, so repeated calls are
       inexpensive even for unordered n. */
    static seq_t *fact_cache = NULL;
    static idx_t fact_cache_size = 0;
    static seq_t *pow4_cache = NULL;
    static idx_t pow4_cache_size = 0;

    /* Ensure factorial cache contains values up to n. */
    if (fact_cache_size <= n) {
        idx_t new_size = n + 1;
        if (!fact_cache)
            fact_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            fact_cache = (seq_t *)realloc(fact_cache, new_size * sizeof(seq_t));

        if (fact_cache_size == 0) {
            fact_cache[0] = (seq_t)1;  /* 0! = 1 */
            fact_cache_size = 1;
        }

        for (idx_t i = fact_cache_size; i <= n; ++i)
            fact_cache[i] = fact_cache[i - 1] * (seq_t)i;

        fact_cache_size = new_size;
    }

    /* Ensure 4^k cache contains values up to (n+1). */
    idx_t need_pow4 = n + 1;
    if (pow4_cache_size <= need_pow4) {
        idx_t new_size = need_pow4 + 1;
        if (!pow4_cache)
            pow4_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        else
            pow4_cache = (seq_t *)realloc(pow4_cache, new_size * sizeof(seq_t));

        if (pow4_cache_size == 0) {
            pow4_cache[0] = (seq_t)1;  /* 4^0 = 1 */
            pow4_cache_size = 1;
        }

        for (idx_t i = pow4_cache_size; i <= need_pow4; ++i)
            pow4_cache[i] = pow4_cache[i - 1] * (seq_t)4;

        pow4_cache_size = new_size;
    }

    /* Compute the term using the closed formula.
       (pow4_cache[n+1] - 1) is divisible by 3. */
    seq_t denom_part = (pow4_cache[n + 1] - (seq_t)1) / (seq_t)3;
    __int128 prod = (__int128)fact_cache[n] * (__int128)denom_part;

    return (seq_t)prod;
}
