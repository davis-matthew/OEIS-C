/*
 * A072863  a(n) = 2^(n-3)*(n^2+3*n+8).
 * https://oeis.org/A072863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..2} binom(n,k)*2^(n-k)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 25, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *pow2    = NULL;
    static idx_t pow2_size    = 0;
    static seq_t *cache   = NULL;
    static idx_t cache_size  = 0;

    if (n >= cache_size) {
        idx_t new_size = n + 1;
        pow2   = (seq_t *)realloc(pow2,   new_size * sizeof(seq_t));
        cache  = (seq_t *)realloc(cache,  new_size * sizeof(seq_t));

        /* compute powers of two up to new_size-1 */
        for (idx_t i = pow2_size; i < new_size; ++i) {
            if (i == 0)
                pow2[i] = (seq_t)1;
            else
                pow2[i] = pow2[i - 1] * (seq_t)2;
        }

        /* compute the sequence values up to new_size-1 */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t val;
            if (i == 0) {
                val = pow2[0];
            } else if (i == 1) {
                val = pow2[1] + pow2[0];
            } else {
                seq_t binom1 = (seq_t)i;
                seq_t binom2 = ((seq_t)i * (i - 1)) / (seq_t)2;
                val = pow2[i] + binom1 * pow2[i - 1] + binom2 * pow2[i - 2];
            }
            cache[i] = val;
        }

        pow2_size  = new_size;
        cache_size = new_size;
    }

    return cache[n];
}
