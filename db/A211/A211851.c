/*
 * A211851  Number of nonnegative integer arrays of length 2n+6 with new
 *          values 0 upwards introduced in order, no three adjacent elements
 *          all unequal, and containing the value n+1.
 * https://oeis.org/A211851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
 *            for n > 5 * Initial values: * a(1) = 196, a(2) = 561, a(3) =
 *            1302, a(4) = 2619, a(5) = 4755
 * Range:     u8 n <= 1, u16 n <= 11, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inttypes.h>   /* for intmax_t */

seq_t a(idx_t n)
{
    /* Cache to store terms up to a maximum of 20 000 (sufficient for n≈10 000). */
    static seq_t cache[20001] = {0};
    static idx_t cache_max = 0;   /* number of terms currently stored, 0 if none */

    if (n == 0) return 0;   /* protection – OEIS offset is 1 */

    /* Handle the first five terms directly. */
    if (n <= 5) {
        /* Constants will be wrapped appropriately for small seq_t types. */
        static const seq_t init[5] = {196, 561, 1302, 2619, 4755};
        return init[n - 1];
    }

    /* Ensure cache contains at least the first five terms. */
    if (cache_max < 5) {
        cache[0] = 196;
        cache[1] = 561;
        cache[2] = 1302;
        cache[3] = 2619;
        cache[4] = 4755;
        cache_max = 5;
    }

    /* Compute missing terms up to n (inclusive). */
    if (n > cache_max) {
        for (idx_t i = 5; i < n; ++i) {
            /* Use signed intermediate to avoid unsigned underflow. */
            intmax_t val = 5 * (intmax_t)cache[i - 1]
                        - 10 * (intmax_t)cache[i - 2]
                        + 10 * (intmax_t)cache[i - 3]
                        - 5  * (intmax_t)cache[i - 4]
                        + (intmax_t)cache[i - 5];
            cache[i] = (seq_t)val;
        }
        cache_max = n;
    }

    return cache[n - 1];
}
