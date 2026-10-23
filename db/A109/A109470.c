/*
 * A109470  Sum of first n noncubes.
 * https://oeis.org/A109470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum of the first n positive integers that are not perfect
 *            cubes (OEIS A109470). The function below computes this by
 *            iterating through the positive integers, skipping cubes, and
 *            accumulating the sum until the desired n terms have been reached
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   is_cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_cube.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, keep safe */
    seq_t sum = 0;
    seq_t val = 0;
    idx_t counted = 0;

    while (counted < n) {
        ++val;
        if (!is_cube(val)) {
            sum += val;
            ++counted;
        }
    }
    return sum;
}
