/*
 * A227959  Number of tilings using monominoes and L-trominoes in 2 X n
 *          chessboard, such that three monominoes cannot occur together in
 *          shape of L-tromino.
 * https://oeis.org/A227959
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) + 2*a(n-3) + 2*a(n-4) + 4*a(n-5) a(0)=1, a(1)=1,
 *            a(2)=4, a(3)=6, a(4)=20
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    const seq_t init[5] = { (seq_t)1, (seq_t)1, (seq_t)4, (seq_t)6, (seq_t)20 };

    if (n < 5)
        return init[(size_t)n];

    size_t idx = (size_t)n;
    if (cache == NULL || idx + 1 > cache_size) {
        size_t new_size = idx + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)            /* allocation failure – return 0 */
            return (seq_t)0;
        cache = tmp;
        for (size_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
        for (size_t i = 0; i < 5 && i < cache_size; ++i)
            cache[i] = init[i];
    }

    for (size_t i = 5; i <= idx; ++i) {
        if (cache[i] == (seq_t)0)
            cache[i] = 4 * cache[i-2] + 2 * cache[i-3] + 2 * cache[i-4] + 4 * cache[i-5];
    }
    return cache[idx];
}
