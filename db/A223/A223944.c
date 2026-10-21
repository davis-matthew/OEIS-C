/*
 * A223944  Number of n X 3 0..1 arrays with diagonals and antidiagonals
 *          unimodal and rows nondecreasing.
 * https://oeis.org/A223944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4a(n-1) - 2a(n-2) - a(n-3) + 5a(n-4) - 8a(n-5) with
 *            initial values a(1)=4, a(2)=16, a(3)=54, a(4)=177, a(5)=596
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 36, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cached_n = 0;

    if (cached_n < 5)
    {
        size_t need = 5;
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        if (!cache) return (seq_t)0;
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)16;
        cache[2] = (seq_t)54;
        cache[3] = (seq_t)177;
        cache[4] = (seq_t)596;
        cached_n = need;
    }

    if ((size_t)n > cached_n)
    {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;
        if (!cache) return (seq_t)0;
        for (size_t i = cached_n; i < new_size; ++i)
        {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t a4 = cache[i - 4];
            seq_t a5 = cache[i - 5];
            cache[i] = 4 * a1 - 2 * a2 - a3 + 5 * a4 - 8 * a5;
        }
        cached_n = new_size;
    }

    return cache[n - 1];
}
