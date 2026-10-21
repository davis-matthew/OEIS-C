/*
 * A088157  Value of (n+1)-th digit in sexagesimal representation of n^n.
 * https://oeis.org/A088157
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = floor( n^n / 60^n ) mod 60
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: multiply a big integer (represented in base 60) by a small integer n,
   keeping only the lowest 'len' digits (i.e. mod 60^len). */
static void mul_small(unsigned short *digits, uint32_t len, uint32_t n)
{
    uint32_t carry = 0;
    for (uint32_t i = 0; i < len; ++i)
    {
        uint32_t prod = (uint32_t)digits[i] * n + carry;
        digits[i] = (unsigned short)(prod % 60);
        carry = prod / 60;
    }
    /* Any carry beyond the lowest 'len' digits is discarded,
       because we only need the number modulo 60^len. */
}

/* Main sequence function.  idx_t and seq_t are provided by the build. */
seq_t a(idx_t n)
{
    /* a(0) = 1 by definition (0^0 = 1). */
    if (n == 0)
        return (seq_t)1;

    /* For 1 <= n < 60, n^n < 60^n so the (n+1)-th digit is 0. */
    if (n < 60)
        return (seq_t)0;

    /* Compute len = n+1 digits (0..n) in base 60. */
    uint32_t len = (uint32_t)n + 1;
    unsigned short *digits = (unsigned short *)calloc(len, sizeof(unsigned short));
    if (!digits)
        return (seq_t)0;  /* unlikely, fallback */

    digits[0] = 1;  /* start with value 1 */

    uint32_t m = (uint32_t)n;  /* small multiplier n */
    /* Repeatedly multiply by n, n times (n^n).  After each
       multiplication we drop higher digits, so we keep the value
       modulo 60^len, sufficient to get the n-th digit. */
    for (uint32_t j = 0; j < m; ++j)
        mul_small(digits, len, m);

    /* digits[n] holds the desired digit (0 <= < 60). */
    seq_t result = (seq_t)digits[n];
    free(digits);
    return result;
}
