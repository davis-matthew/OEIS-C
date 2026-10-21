/*
 * A220629  Number of ways to reciprocally link elements of an n X 5 array
 *          either to themselves or to exactly two horizontal or antidiagonal
 *          neighbors.
 * https://oeis.org/A220629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 9*a(n-2) + 7*a(n-3) + 2*a(n-4) - a(n-5).
 *            Initial terms: a(1)=1, a(2)=8, a(3)=44, a(4)=257, a(5)=1481
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static const seq_t init[5] = {
        (seq_t)1,
        (seq_t)8,
        (seq_t)44,
        (seq_t)257,
        (seq_t)1481
    };

    if (n == 0)
        return 0;

    if (cache_len < n) {
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            if (i <= 5) {
                cache[i - 1] = init[i - 1];
            } else {
                seq_t a1 = cache[i - 2];
                seq_t a2 = cache[i - 3];
                seq_t a3 = cache[i - 4];
                seq_t a4 = cache[i - 5];
                seq_t a5 = cache[i - 6];
                cache[i - 1] = (seq_t)4 * a1
                               + (seq_t)9 * a2
                               + (seq_t)7 * a3
                               + (seq_t)2 * a4
                               - a5;
            }
        }
        cache_len = n;
    }
    return cache[n - 1];
}
