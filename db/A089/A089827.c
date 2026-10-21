/*
 * A089827  Number of subsets of {1,.., n} containing no twin prime pairs.
 * https://oeis.org/A089827
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n and n-2 are primes then (if n=7 then a(n-1)*5/3 else
 *            a(n-1)*3/2) else a(n-1)*2 Initial values: a(1)=2, a(2)=4
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n < 1) return 0;            /* safety for offsets below 1 */

    /* Ensure cache capacity */
    if (cache_size < n + 1) {
        idx_t new_size = cache_size ? cache_size * 2 : 16;
        while (new_size <= n) new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort();  /* OOM */
        for (idx_t i = cache_size; i < new_size; ++i) cache[i] = 0;
        cache_size = new_size;
    }

    /* Compute values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != 0) continue;     /* already computed */

        if (i == 1) {
            cache[i] = (seq_t)2;
        } else if (i == 2) {
            cache[i] = (seq_t)4;
        } else { /* i >= 3 */
            if (is_prime(i) && is_prime(i - 2)) {
                if (i == 7) {
                    cache[i] = (cache[i - 1] * (seq_t)5) / (seq_t)3;
                } else {
                    cache[i] = (cache[i - 1] * (seq_t)3) / (seq_t)2;
                }
            } else {
                cache[i] = cache[i - 1] * (seq_t)2;
            }
        }
    }

    return cache[n];
}
