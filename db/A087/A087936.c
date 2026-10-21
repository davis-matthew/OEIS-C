/*
 * A087936  Perrin sequence of order 6.
 * https://oeis.org/A087936
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-5) + a(n-6) with a(0)=6, a(1)=a(2)=a(3)=a(4)=0,
 *            a(5)=5
 * Range:     u8 n <= 42, u16 n <= 86, u32 n <= 175, u64 n <= 350,
 *            u128 n <= 701, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t base[6] = {6, 0, 0, 0, 0, 5};

    if (n < 6)                   /* immediately return the base values */
        return base[n];

    if (cache_len == 0) {         /* first call: allocate and fill 0..5 */
        cache_len = 6;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache) return 0;     /* allocation failed – unlikely in tests */
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = base[i];
    }

    if (n >= cache_len) {         /* grow array and compute missing terms */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
        if (!tmp) return 0;       /* allocation failed */
        cache = tmp;
        for (idx_t i = cache_len; i <= n; ++i) {
            cache[i] = cache[i - 5] + cache[i - 6];
        }
        cache_len = n + 1;
    }

    return cache[n];
}
