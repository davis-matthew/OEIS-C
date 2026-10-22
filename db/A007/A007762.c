/*
 * A007762  Number of domino tilings of a certain region.
 * https://oeis.org/A007762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, unkn, more
 * Formula:   a(n) = A006318(n-1) * A104550(n), where A006318(m): (m+1)*S(m) =
 *            3*(2m-1)*S(m-1) - (m-2)*S(m-2), S(0) = 1, S(1) = 2; A104550(n) =
 *            Sum_{k=0..n-1} binomial(n-1+k, 2k) * binomial(2k+1, k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   b104550, schroeder
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/b104550.h"
#include "../utils/schroeder.h"

seq_t a(idx_t n) {
    return schroeder(n - 1) * b104550(n);
}
