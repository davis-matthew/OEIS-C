/*
 * A085435  Resultant of the polynomial x^n-1 and the Chebyshev polynomial of
 *          the second kind U_2(x).
 * https://oeis.org/A085435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n - 2^n - (-2)^n + (-1)^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 we return 0 arbitrarily. */
    if (n == 0) {
        return 0;
    }

    /* 4^n is common to all cases. */
    seq_t pow4 = seq_pow(4, n);

    if (n & 1ULL) {            /* n is odd : a(n) = 4^n - 1 */
        return pow4 - 1;
    } else {                   /* n is even : a(n) = 4^n - 2^(n+1) + 1 */
        seq_t pow2 = seq_pow(2, n);          /* 2^n */
        seq_t double_pow2 = pow2 + pow2;     /* 2 * 2^n = 2^(n+1) */
        return pow4 - double_pow2 + 1;
    }
}
