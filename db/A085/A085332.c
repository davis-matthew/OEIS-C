/*
 * A085332  Prime powers that are sums of two positive cubes.
 * https://oeis.org/A085332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th smallest integer that is a prime power of the *
 *            form 2^(3k+1) or 3^(3k+2) (k ≥ 0). These are exactly the prime
 *            powers * that can be written as the sum of two positive cubes
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* cache of already computed terms */
    static idx_t   cache_size   = 0;
    static idx_t   cache_capacity = 0;
    static seq_t  *cache          = NULL;

    /* current values of the two geometric sequences */
    static seq_t  val2 = (seq_t)2;   /* 2^(1)   */
    static seq_t  val3 = (seq_t)9;   /* 3^(2)   */
    static int    initialized = 0;

    if (!initialized) {
        val2 = (seq_t)2;
        val3 = (seq_t)9;
        initialized = 1;
    }

    /* generate terms until we have at least n of them */
    while (cache_size < n) {
        /* enlarge the cache if necessary */
        if (cache_capacity == 0 || cache_size == cache_capacity) {
            idx_t new_cap = (cache_capacity == 0) ? 32 : (cache_capacity << 1);
            seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
            cache = tmp;
            cache_capacity = new_cap;
        }

        if (val2 <= val3) {
            cache[cache_size++] = val2;
            val2 *= (seq_t)8;   /* multiply by 2^3 to advance the exponent by 3 */
        } else {
            cache[cache_size++] = val3;
            val3 *= (seq_t)27;  /* multiply by 3^3 to advance the exponent by 3 */
        }
    }

    return cache[(size_t)(n - 1)];
}
