/*
 * A159989  Catalan numbers read modulo 12.
 * https://oeis.org/A159989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Catalan(n) mod 12
 * Range:     u8 n <= 118, u16 n <= 118, u32 n <= 118, u64 n <= 118,
 *            u128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Compute a(n) = C_n mod 12, where C_n is the nth Catalan number.  */

seq_t a(idx_t n)
{
    /* static cache: value for every m <= cache_size-1 */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache has enough space.  If the cache grows, reallocate
       to accommodate all values up to n.  */
    if (n + 1 > cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) return 0;  /* unable to allocate; return 0 as fallback */

        cache = new_cache;

        /* If this is the first time, initialize the first term.  */
        if (cache_size == 0) {
            cache[0] = 1;
            cache_size = 1;
        }

        /* Compute new terms using the convolution recurrence
           C_m = sum_{i=0}^{m-1} C_i * C_{m-1-i}.  All arithmetic
           is performed modulo 12, so values remain within [0,11].  */
        const seq_t MOD = 12;
        for (idx_t m = cache_size; m <= n; ++m) {
            seq_t sum = 0;
            for (idx_t i = 0; i < m; ++i) {
                seq_t prod = cache[i] * cache[m - 1 - i];
                /* The product is at most 11*11=121, so it fits easily
                   into seq_t.  Modulo reduction keeps the intermediate
                   sum small.  */
                sum += prod % MOD;
                sum %= MOD;
            }
            cache[m] = sum % MOD;
        }
        cache_size = n + 1;
    }

    return cache[n] % 12;
}
