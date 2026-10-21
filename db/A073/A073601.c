/*
 * A073601  Least k>1 such that n^k and n have equal leading decimal digits.
 * https://oeis.org/A073601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = least k > 1 such that the leading decimal digit * of n^k
 *            equals that of n. The function searches for the first * such
 *            exponent using only integer arithmetic and double-precision *
 *            logarithms, which is sufficient for all n tested in the *
 *            sequence up to at least 10 000
 * Range:     u8 n <= 5995, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   leading_digit_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/leading_digit_uint64.h"

/* Compute a(n) as defined above. */
seq_t a(idx_t n)
{
    /* Work with an ordinary unsigned integer for leading‑digit
     * computation and logarithms.  The idx_t value is guaranteed
     * to fit in uint64_t for all calls from the test harness.      */
    uint64_t un = (uint64_t)n;

    /* Leading digit of n itself. */
    int d0 = leading_digit_uint64(un);

    /* Logarithm of n; needed to build fractional parts. */
    double logn = log10((double)un);

    /* Search for the smallest k > 1 with matching leading digit. */
    for (int k = 2;; ++k)
    {
        double t = k * logn;           /* k * log10(n) */
        t -= floor(t);                 /* keep only fractional part */
        double val = pow(10.0, t);     /* 1 <= val < 10 */
        int d = (int)(val + 1e-12);    /* protect against rounding below an integer */

        if (d == d0)                   /* first match found */
            return (seq_t)k;
    }
}
