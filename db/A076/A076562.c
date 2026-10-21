/*
 * A076562  a(1)=3; a(n>1)= greatest prime divisor of a(n-1) + n.
 * https://oeis.org/A076562
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 231, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   largest_prime_divisor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(1) = 3
    a(n) = greatest prime divisor of a(n‑1) + n   for n > 1
*/

#include <stdint.h>
#include <stdlib.h>
#include "../utils/largest_prime_divisor.h"   /* seq_t largest_prime_divisor(uint64_t) */

seq_t a(idx_t n)
{
    /* static cache: indices 1 … cache_last are filled   */
    static seq_t *cache    = NULL;
    static idx_t cache_size  = 0;   /* allocated size  */
    static idx_t cache_last  = 0;   /* last computed index  */

    if (n < 1)                 /* undefined: return 0  */
        return (seq_t)0;

    /* ensure enough space for index n */
    if ((size_t)n + 1 > (size_t)cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)           /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        cache_size = (idx_t)new_size;
    }

    /* compute missing terms */
    for (idx_t i = cache_last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)3;
        } else {
            uint64_t sum = (uint64_t)cache[i - 1] + (uint64_t)i;
            uint64_t lpf = largest_prime_divisor(sum);
            cache[i] = (seq_t)lpf;
        }
    }
    cache_last = n;

    return cache[n];
}
