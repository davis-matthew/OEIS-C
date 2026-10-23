/*
 * A024641  n written in fractional base 7/4.
 * https://oeis.org/A024641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = integer formed by concatenating the digits of n written
 *            in * fractional base 7/4 (digits 0..6). The digits are taken as
 *            ordinary decimal * digits. For n=0 the value is 0
 * Range:     u8 n <= 13, u16 n <= 90, u32 n <= 1336, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Handle the trivial case directly.  This also avoids allocating an
       array when the answer is just 0. */
    if (n == 0)
        return 0;

    /* The representation of an integer in base 7/4 can be built by
       repeatedly extracting the remainder modulo 7.  Given the current value
       N, let d = N mod 7.  Then the remaining part is (N - d)/7 * 4,
       which is guaranteed to be an integer because (N - d) is divisible by 7.
       This process terminates after O(log_{7/4} N) steps. */
    unsigned char digits[128];   /* 128 digits is ample for any 64‑bit integer. */
    size_t len = 0;

    while (n > 0) {
        unsigned int d = n % 7u;          /* digit 0..6 */
        digits[len++] = (unsigned char)d;
        n = ((n - d) / 7u) * 4u;          /* next value */
    }

    /* Convert the digit array into the decimal integer that the sequence
       expects, i.e. concatenate the digits in reverse order. */
    seq_t result = 0;
    while (len > 0) {
        result = result * 10 + digits[--len];
    }
    return result;
}
