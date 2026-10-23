/*
 * A199320  a(n) = 5*6^n+1.
 * https://oeis.org/A199320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*6^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   pow_seq6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_seq6.h"

/* Main sequence function required by the build. */
seq_t a(idx_t n)
{
    seq_t pow6 = pow_seq6(n);
    return 5 * pow6 + 1;
}
