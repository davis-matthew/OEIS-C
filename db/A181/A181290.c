/*
 * A181290  The sum of the lengths of the 2-compositions of n. A 2-composition
 *          of n is a nonnegative matrix with two rows, such that each column
 *          has at least one nonzero entry and whose entries sum up to n. The
 *          length of the 2-composition is the number of columns.
 * https://oeis.org/A181290
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} k*A181289(n,k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the rational generating function:
   A(z) = z*(2-z)*(1-z)^2 / (1-4*z+2*z^2)^2
   ⇒  a_n = 8*a_{n-1} - 20*a_{n-2} + 16*a_{n-3} - 4*a_{n-4}   (n ≥ 5)
   with a_0 = 0, a_1 = 2, a_2 = 11, a_3 = 52, a_4 = 227. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[i] cached for i < cache_len */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* ensure the cache is large enough for indices 0..n */
    if (n + 1 > cache_len) {
        /* allocate space for at least n+1 elements,
           but always allocate at least 5 to hold the base values. */
        idx_t new_len = (n + 1 >= 5 ? n + 1 : 5);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache)       /* allocation failure: fall back to zero */
            return (seq_t)0;
        cache = new_cache;

        /* compute values from cache_len up to new_len-1 */
        for (idx_t i = cache_len; i < new_len; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)0;
            } else if (i == 1) {
                cache[i] = (seq_t)2;
            } else if (i == 2) {
                cache[i] = (seq_t)11;
            } else if (i == 3) {
                cache[i] = (seq_t)52;
            } else if (i == 4) {
                cache[i] = (seq_t)227;
            } else {
                /* recurrence for i ≥ 5:
                   a[i] = 8*a[i-1] - 20*a[i-2] + 16*a[i-3] - 4*a[i-4] */
                seq_t term8  = (seq_t)8  * cache[i - 1];
                seq_t term20 = (seq_t)20 * cache[i - 2];
                seq_t term16 = (seq_t)16 * cache[i - 3];
                seq_t term4  = (seq_t)4  * cache[i - 4];

                /* combine positive and negative parts separately to avoid
                   intermediate overflow in signed/unsigned wrap-arounds. */
                seq_t pos = term8 + term16;
                seq_t neg = term20 + term4;
                cache[i] = pos - neg;
            }
        }
        cache_len = new_len;
    }

    return cache[n];
}
