/*
 * A104611  Write the natural numbers in base 7 in a triangle with k digits in
 *          the k-th row, as shown below. Sequence gives the leading diagonal.
 * https://oeis.org/A104611
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the (n*(n+1)/2)-th digit of the concatenated base‑7 *
 *            representations of positive integers. The digit is taken from
 *            the * last position of the row that contains that digit
 * Range:     u8 n <= 19, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* below offset – arbitrary */

    /* Triangular position of the desired digit. */
    seq_t t = (seq_t)n * ((seq_t)n + 1) / 2;

    /* Find the block of numbers that contains this digit. */
    idx_t d = 1;                /* current digit length */
    seq_t pow = 1;              /* 7^(d-1) */
    seq_t cumulative = 0;       /* digits processed before this block */
    seq_t blockDigits = (seq_t)6 * pow * (seq_t)d;

    while (t > cumulative + blockDigits) {
        cumulative += blockDigits;
        d++;
        pow *= (seq_t)7;
        blockDigits = (seq_t)6 * pow * (seq_t)d;
    }

    /* Position of the digit within the block (0‑based). */
    seq_t offset = t - cumulative - 1;
    seq_t numberIndex = offset / (seq_t)d;
    seq_t digitIndex = offset % (seq_t)d;

    /* The integer of which we need a digit. */
    seq_t m = pow + numberIndex;

    /* Power of 7 that isolates the desired digit. */
    seq_t power = pow;
    for (idx_t i = 0; i < (idx_t)digitIndex; ++i) {
        power /= (seq_t)7;
    }

    /* Extract the digit. */
    seq_t digit = (m / power) % (seq_t)7;
    return digit;
}
