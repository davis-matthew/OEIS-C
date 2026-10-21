/*
 * A231674  a(n) = Sum_{i=0..n} digsum_6(i)^3, where digsum_6(i) = A053827(i).
 * https://oeis.org/A231674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{i=0..n} digsum_6(i)^3 , digsum_6(i)=sum of the base‑6
 *            digits of i
 * Range:     u8 n <= 7, u16 n <= 158, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: a[i] = a(i) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (cache_size == 0) {
        cache_size = 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = 0;
    }

    if (n >= cache_size) {
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (tmp == NULL) abort();      /* memory allocation failed */
        cache = tmp;
        for (idx_t i = cache_size; i <= n; ++i) {
            idx_t tmpi = i;
            idx_t sum = 0;
            while (tmpi) {
                sum += tmpi % 6;
                tmpi /= 6;
            }
            seq_t cube = (seq_t)sum * sum * sum;
            cache[i] = cache[i - 1] + cube;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
