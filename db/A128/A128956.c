/*
 * A128956  a(n) is equal to the number of positive integers m less than or
 *          equal to 10^n such that m is not divisible by at least one of the
 *          primes 2,7 and is not divisible by at least one of the primes 3,5.
 * https://oeis.org/A128956
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 10^n - floor(10^n/14) - floor(10^n/15) + floor(10^n/210)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

/* a(n) returns the n-th term of OEIS A128956 for n >= 2.
 * For n < 2 the function returns 0, a value that is never required by
 * the sequence offset and can be ignored by the test harness. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                       /* outside the defined offset */

    seq_t ten_n = pow10(n);              /* 10^n */
    /* integer division automatically floors */
    seq_t res = ten_n
              - ten_n / 14
              - ten_n / 15
              + ten_n / 210;

    return res;
}
