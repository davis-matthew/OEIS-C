/*
 * A077585  a(n) = 2^(2^n - 1) - 1.
 * https://oeis.org/A077585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2^n - 1) - 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed via recurrence a(0)=0, a(n)=2*a(n-1)^2+4*a(n-1)+1 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;

    if (n + 1 > cache_sz) {
        size_t new_sz = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_sz * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure fallback */
        cache = tmp;
        if (cache_sz == 0) {
            cache[0] = (seq_t)0;
            cache_sz = 1;
        }
        for (size_t i = cache_sz; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t res = prev * prev;      /* prev^2 */
            res = res * (seq_t)2;         /* 2*prev^2 */
            res = res + prev * (seq_t)4;  /* +4*prev */
            res = res + (seq_t)1;         /* +1 */
            cache[i] = res;
        }
        cache_sz = new_sz;
    }
    return cache[n];
}
