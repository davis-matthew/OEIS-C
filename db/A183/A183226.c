/*
 * A183226  Sum of digits of (2^n) in base 5, also sum of digits of (10^n) in
 *          base 5.
 * https://oeis.org/A183226
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = sum of digits of 2^n in base 5 (computed by repeated
 *            multiplication in base‑5)
 * Range:     u8 n <= 275, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* handle the trivial case */
    if (n == 0) {
        return (seq_t)1;
    }

    /* digits array stores the representation of 2^k in base 5.
       The maximum number of base‑5 digits needed for 2^n is
       floor(n*log5(2)) + 1  <  n  for all n >= 1, so an array
       of size n+1 is sufficient. */
    size_t max_len = (size_t)n + 1;
    unsigned char *digits = (unsigned char *)malloc(max_len);
    if (!digits) {
        /* allocation failed – return 0 as a fallback */
        return (seq_t)0;
    }

    size_t len = 1;          /* current number of digits */
    digits[0] = 1;           /* 2^0 = 1 */

    /* iterate from 1 to n, multiplying the current number by 2
       each time and updating the base‑5 digit array. */
    for (idx_t k = 1; k <= n; ++k) {
        unsigned int carry = 0;
        for (size_t i = 0; i < len; ++i) {
            unsigned int temp = (unsigned int)digits[i] * 2 + carry;
            digits[i] = (unsigned char)(temp % 5);
            carry = temp / 5;
        }
        if (carry) {
            digits[len] = (unsigned char)carry;
            ++len;
        }
    }

    /* compute the sum of the digits of 2^n in base 5 */
    seq_t sum = 0;
    for (size_t i = 0; i < len; ++i) {
        sum += (seq_t)digits[i];
    }

    free(digits);
    return sum;
}
