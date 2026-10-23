/*
 * A032087  Number of reversible strings with n beads of 4 colors. If more
 *          than 1 bead, not palindromic.
 * https://oeis.org/A032087
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(1) = 4 * For even n>1: a(n) = 2^(n-1) * (2^n - 1) * For odd
 *            n>1: a(n) = 2^(2n-1) - 2^n * The implementation uses the
 *            provided seq_t pow2(idx_t exp) routine
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)4;

    if ((n & 1ULL) == 0ULL) {          /* even n */
        seq_t t1 = pow2(n - 1);
        seq_t t2 = pow2(n);
        return t1 * (t2 - 1);
    } else {                           /* odd n > 1 */
        /* 2n - 1 can be computed without overflow for practical n */
        seq_t t1 = pow2(n + n - 1);   /* 2^(2n-1) */
        seq_t t2 = pow2(n);           /* 2^n */
        return t1 - t2;
    }
}
