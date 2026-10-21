/*
 * A206266  Number of (n+1) X 8 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   via recurrence a(1)=154, a(2)=174, a(3)=503, a(4)=1296,
 *            a(5)=3011, a(6)=6444, a(7)=12878, a(8)=24319, a(9)=43766,
 *            a(10)=75591, a(11)=125978
 *            a(n)=8*a(n-1)-27*a(n-2)+48*a(n-3)-42*a(n-4)+42*a(n-6)-48*a(n-7)+27*a(n-8)-8*a(n-9)+a(n-10)
 *            (n>11)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 54, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial values for n = 1 .. 11 */
    static const seq_t init[11] = {
        154,
        174,
        503,
        1296,
        3011,
        6444,
        12878,
        24319,
        43766,
        75591,
        125978
    };

    if (n <= 11) {
        return init[n-1];
    }

    /* Dynamic cache for computed terms up to the largest n seen */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;          /* highest index stored */

    size_t target = (size_t)n;

    if (cache == NULL) {
        size_t cap = target > 11 ? target : 11;
        cache = (seq_t*)calloc(cap + 1, sizeof(seq_t));   /* index 0 unused */
        for (size_t i = 1; i <= 11; ++i) {
            cache[i] = init[i-1];
        }
        cache_size = 11;
    } else if (target > cache_size) {
        cache = (seq_t*)realloc(cache, (target + 1) * sizeof(seq_t));
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cache_size + 1; i <= target; ++i) {
        seq_t pos = 8*cache[i-1] + 48*cache[i-3] + 42*cache[i-6] + 27*cache[i-8] + cache[i-10];
        seq_t neg = 27*cache[i-2] + 42*cache[i-4] + 48*cache[i-7] + 8*cache[i-9];
        cache[i] = pos - neg;   /* pos ≥ neg by construction */
    }
    cache_size = target;

    return cache[target];
}
