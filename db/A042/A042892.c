/*
 * A042892  Numerators of continued fraction convergents to sqrt(978).
 * https://oeis.org/A042892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 236674*a(n-10) - a(n-20) for n >= 20; Initial values:
 *            a(0)=31, a(1)=94, a(2)=125, a(3)=219, a(4)=344, a(5)=10539,
 *            a(6)=10883, a(7)=21422, a(8)=32305, a(9)=118337, a(10)=7369199,
 *            a(11)=22225934, a(12)=29595133, a(13)=51821067, a(14)=81416200,
 *            a(15)=2494307067, a(16)=2575723267, a(17)=507003...
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static const seq_t init_vals[20] = {
    31, 94, 125, 219, 344,
    10539, 10883, 21422, 32305, 118337,
    7369199, 22225934, 29595133, 51821067, 81416200,
    2494307067, 2575723267, 5070030334, 7645753601, 28007291137
};

seq_t a(idx_t n)
{
    if (cache == NULL) {
        cache_len = 20;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < 20; ++i)
            cache[i] = init_vals[i];
    }
    if (n >= cache_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        idx_t old_len = cache_len;
        cache_len = n + 1;
        for (idx_t i = old_len; i <= n; ++i) {
            if (i < 20) {
                cache[i] = init_vals[i];
            } else {
                cache[i] = (seq_t)236674 * cache[i - 10] - cache[i - 20];
            }
        }
    }
    return cache[n];
}
