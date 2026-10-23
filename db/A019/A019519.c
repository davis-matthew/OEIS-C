/*
 * A019519  Concatenate odd numbers.
 * https://oeis.org/A019519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = concat_{k=1..n} (2k-1) (decimal concatenation of odd
 *            integers)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 16,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1, return 0 */

    /* static cache: cache[i] holds a(i+1)  (0‑based index) */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;

    /* already computed?  */
    if (n <= cache_size) {
        return cache[n - 1];
    }

    /* grow cache capacity if necessary */
    if (n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap : 1;
        while (new_cap < (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* start from the last cached value (or 0 if nothing cached) */
    seq_t current = cache_size ? cache[cache_size - 1] : 0;

    /* compute terms up to n, storing each in the cache */
    for (idx_t i = cache_size + 1; i <= n; ++i) {
        seq_t odd = (seq_t)i * (seq_t)2 - (seq_t)1;   /* 2*i-1 */

        if (i == 1) {
            current = odd;                          /* first term */
        } else {
            seq_t mult = pow10((idx_t)digit_len(odd));
            current = current * mult + odd;
        }

        cache[i - 1] = current;
    }

    cache_size = (size_t)n;
    return cache[n - 1];
}
