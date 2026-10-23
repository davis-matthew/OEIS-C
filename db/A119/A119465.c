/*
 * A119465  Consider the standard game of Nim with 3 heaps and make a list of
 *          the losing positions (x,y,z) with x <= y <= z sorted by sum, ties
 *          broken by putting smallest value of x first, then y, then z;
 *          sequence gives y values.
 * https://oeis.org/A119465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = y of the n‑th losing Nim position (x,y,z) with x <= y <=
 *            z, sorted by sum, ties by x then y
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* n is 1‑based */
    if (n == 0) return (seq_t)0; /* undefined, but keep simple */

    idx_t target = n;
    idx_t count   = 0;

    for (idx_t sum = 0; ; ++sum) {
        /* x ranges from 0 to sum/3  (since x <= y <= z) */
        for (idx_t x = 0; x <= sum / 3; ++x) {
            /* y ranges from x to (sum - x) / 2  (since y <= z) */
            for (idx_t y = x; y * 2 <= sum - x; ++y) {
                idx_t z = sum - x - y;   /* z >= y by loop guard */

                /* check Nim losing condition */
                if ((x ^ y ^ z) == 0) {
                    ++count;
                    if (count == target)
                        return (seq_t)y;   /* return the y component */
                }
            }
        }
    }
}
