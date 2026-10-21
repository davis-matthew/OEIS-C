/*
 * A217632  Number of nX3 arrays of the minimum value of corresponding
 *          elements and their horizontal and vertical neighbors in a random
 *          0..1 nX3 array.
 * https://oeis.org/A217632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) +9*a(n-2) -2*a(n-3) -17*a(n-4) -4*a(n-5) +
 *            8*a(n-6) -3*a(n-7) +a(n-8) -3*a(n-9) -2*a(n-10) +4*a(n-11)
 *            Initial values: a(0)=0, a(1)=4, a(2)=16, a(3)=66, a(4)=244,
 *            a(5)=968, a(6)=3726, a(7)=14520, a(8)=56352, a(9)=218978,
 *            a(10)=850620, a(11)=3304624
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Precomputed values for 0 … 11 */
    static const seq_t base_values[12] =
        {0, 4, 16, 66, 244, 968, 3726, 14520, 56352, 218978, 850620, 3304624};

    if (n < 12)
        return base_values[(size_t)n];

    /* Dynamic cache for all values computed so far */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static int initialized = 0;

    if (!initialized) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * 12);
        cache_size = 12;
        for (size_t i = 0; i < 12; ++i)
            cache[i] = base_values[i];
        initialized = 1;
    }

    /* Extend cache if necessary */
    if ((size_t)n + 1 > cache_size) {
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        cache = tmp;
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            seq_t pos = cache[i - 1] * (seq_t)2;
            pos += cache[i - 2] * (seq_t)9;
            pos += cache[i - 6] * (seq_t)8;
            pos += cache[i - 8];
            pos += cache[i - 11] * (seq_t)4;

            seq_t neg = cache[i - 3] * (seq_t)2;
            neg += cache[i - 4] * (seq_t)17;
            neg += cache[i - 5] * (seq_t)4;
            neg += cache[i - 7] * (seq_t)3;
            neg += cache[i - 9] * (seq_t)3;
            neg += cache[i - 10] * (seq_t)2;

            cache[i] = pos - neg;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
