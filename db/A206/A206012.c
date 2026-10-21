/*
 * A206012  Modular recursion: a(0)=a(1)=a(2)=a(3)=1, thereafter: a(n) equals
 *          a(n - 2) + a(n - 3) when n = 0 mod 5, a(n - 1) + a(n - 3) when n =
 *          1 mod 5, a(n - 1) + a(n - 2) when n = 2 mod 5, a(n - 1) + a(n - 4)
 *          when n = 3 mod 5, and a(n - 1) + a(n - 2) + a(n - 3) otherwise.
 * https://oeis.org/A206012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0)=a(1)=a(2)=a(3)=1; for n≥4: * if n mod 5 == 0:
 *            a(n)=a(n-2)+a(n-3) * if n mod 5 == 1: a(n)=a(n-1)+a(n-3) * if n
 *            mod 5 == 2: a(n)=a(n-1)+a(n-2) * if n mod 5 == 3:
 *            a(n)=a(n-1)+a(n-4) * otherwise : a(n)=a(n-1)+a(n-2)+a(n-3)
 * Range:     u8 n <= 17, u16 n <= 32, u32 n <= 62, u64 n <= 121,
 *            u128 n <= 240, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Handle the base cases */
    if (n < 4)
        return (seq_t)1;

    /* Static cache to store computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Initialize cache with first four terms if empty */
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (!cache)
            exit(1);
        cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)1;
    }

    /* If the requested term is already cached, return it */
    if (cache_size > n)
        return cache[n];

    /* Ensure there is enough space for all terms up to n */
    if (cache_size <= n) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        if (!tmp)
            exit(1);
        cache = tmp;
    }

    /* Compute missing terms starting from the first index after the cached block */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t val;
        switch (i % 5) {
            case 0:
                val = cache[i - 2] + cache[i - 3];
                break;
            case 1:
                val = cache[i - 1] + cache[i - 3];
                break;
            case 2:
                val = cache[i - 1] + cache[i - 2];
                break;
            case 3:
                val = cache[i - 1] + cache[i - 4];
                break;
            default: /* i % 5 == 4 */
                val = cache[i - 1] + cache[i - 2] + cache[i - 3];
                break;
        }
        cache[i] = val;
    }

    /* Update the number of cached terms */
    cache_size = n + 1;
    return cache[n];
}
