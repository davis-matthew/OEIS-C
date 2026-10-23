/*
 * A104616  Write the natural numbers in base 13 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the digit at position n(n+1)/2 in the concatenation of
 *            the base‑13 representations of the natural numbers. This digit
 *            is the n‑th element on the leading diagonal of the triangle with
 *            k digits in row k
 * Range:     u8 n <= 21, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   cumulative_digits, find_number, pow13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ----------------------------------------------------------- */
#include <stdint.h>
#include "../utils/cumulative_digits.h"
#include "../utils/find_number.h"
#include "../utils/pow13.h"

/* ----------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* position (triangular number) */
    seq_t pos = (seq_t)n * ((seq_t)n + 1) / 2;

    /* find the number that contains this position */
    idx_t num = find_number(pos);
    seq_t prev = cumulative_digits((seq_t)(num - 1));
    idx_t idx_within = (idx_t)(pos - prev - 1);   /* 0‑based index inside the number */

    /* obtain the base‑13 digits of `num` */
    seq_t tmp = (seq_t)num;
    seq_t digits[64];            /* enough for many digits */
    int d = 0;
    while (tmp > 0) {
        digits[d++] = tmp % 13;
        tmp /= 13;
    }

    /* digits are stored LSB first; return the required one */
    return digits[d - 1 - (int)idx_within];
}
