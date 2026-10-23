/*
 * A159984  Catalan numbers read modulo 5 .
 * https://oeis.org/A159984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Catalan numbers modulo 5.  a(n) = Catalan(n) mod 5.  */
#include <stdlib.h>

/* Compute the nth Catalan number modulo 5.  */
seq_t a(idx_t n)
{
    /* static cache of computed values  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of elements already computed (size) */

    /* Ensure the cache is allocated and large enough. */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;   /* C_0 = 1 */
    } else if (cache_size <= n) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* compute new entries up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t sum = 0;
            /* Catalan recurrence: C_i = sum_{j=0}^{i-1} C_j * C_{i-1-j} */
            for (idx_t j = 0; j < i; ++j) {
                sum = (sum + cache[j] * cache[i - 1 - j]) % 5;
            }
            cache[i] = sum;
        }
        cache_size = new_size;
    }

    /* Return the desired value. */
    return cache[n];
}
