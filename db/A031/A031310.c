/*
 * A031310  Write n in base 10, complement each digit and juxtapose.
 * https://oeis.org/A031310
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 9 - d_n, where d_n is the n‑th digit of Champernowne’s
 *            concatenation 0.123456789101112… (counting from position 1 after
 *            the decimal point). In other words, write n in decimal,
 *            complement each digit, and append the result
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* 0‑based offset into the Champernowne string */
    idx_t pos = n - 1;

    /* Find the block of numbers with the same number of digits that
       contains the desired digit. */
    idx_t d = 1;           /* current number of digits of numbers */
    idx_t count = 9;       /* how many numbers have d digits */

    while (1) {
        idx_t block_len = d * count;          /* digits contributed by this block */
        if (pos < block_len) break;
        pos -= block_len;
        ++d;
        count *= 10;                           /* next block has ten times as many numbers */
    }

    /* Position within the current block. */
    idx_t number_index = pos / d;              /* which number in this block (0‑based) */
    idx_t digit_index  = pos % d;              /* which digit of that number (0‑leftmost) */

    /* Locate the exact number and extract the requested digit. */
    seq_t first  = pow10(d - 1);                    /* first number in this block */
    seq_t number = first + number_index;           /* the number itself */
    seq_t divisor = pow10(d - digit_index - 1);     /* 10^(digits to ignore on the right) */
    seq_t digit = (number / divisor) % 10;          /* the digit at position digit_index */

    /* Complement the digit. */
    return 9 - digit;
}
