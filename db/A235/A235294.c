/*
 * A235294  Number of (n+1) X (4+1) 0..3 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 3
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A235294
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 17*a(n-2) - 57*a(n-3) - 86*a(n-4) + 372*a(n-5)
 *            + 76*a(n-6) - 972*a(n-7) + 360*a(n-8) + 864*a(n-9) -
 *            576*a(n-10); initial values: a(1)=744, a(2)=1132, a(3)=1736,
 *            a(4)=3184, a(5)=5784, a(6)=12328, a(7)=25592, a(8)=60856,
 *            a(9)=138744, a(10)=356632
 * Range:     u8 none, u16 n <= 8, u32 n <= 19, u64 n <= 40, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    (seq_t)744, (seq_t)1132, (seq_t)1736, (seq_t)3184, (seq_t)5784,
    (seq_t)12328, (seq_t)25592, (seq_t)60856, (seq_t)138744, (seq_t)356632
};

static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of allocated elements (+1 for index 0) */

seq_t a(idx_t n)
{
    if (n <= 10)
        return init[(size_t)n - 1];

    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;   /* extra slot for index 0 */
        if (cache == NULL) {
            cache = (seq_t*)calloc(new_len, sizeof(seq_t));
        } else {
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        }
        /* fill the uninitialized part with the first 10 terms if needed */
        if (cache_len == 0) {
            for (size_t i = 1; i <= 10; ++i)
                cache[i] = init[i-1];
        } else if (cache_len < 11) {
            for (size_t i = cache_len; i <= 10; ++i)
                cache[i] = init[i-1];
        }

        /* compute remaining terms up to n */
        for (size_t i = (cache_len > 10 ? cache_len : 11); i <= (size_t)n; ++i) {
            seq_t val = 0;
            val += 3 * cache[i-1];
            val += 17 * cache[i-2];
            val -= 57 * cache[i-3];
            val -= 86 * cache[i-4];
            val += 372 * cache[i-5];
            val += 76 * cache[i-6];
            val -= 972 * cache[i-7];
            val += 360 * cache[i-8];
            val += 864 * cache[i-9];
            val -= 576 * cache[i-10];
            cache[i] = val;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}
