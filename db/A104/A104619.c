/*
 * A104619  Write the natural numbers in base 16 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104619
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = hex digit at position n(n+1)/2 in the infinite stream *
 *            obtained by concatenating the hex representations of 1,2,3,... .
 *            * The leading diagonal of the triangle is the last digit of each
 *            row, * which is precisely the digit at that triangular position
 * Range:     u8 n <= 30, u16 n <= 94, u32 n <= 94, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Position of the desired digit in the stream (1‑based) */
    idx_t k = (idx_t)n * ((idx_t)n + 1) / 2;
    idx_t pos = k;                        /* remaining position */

    idx_t pow16 = 1;   /* 16^(len-1) */
    idx_t len   = 1;   /* current digit length */

    /* Find the interval of numbers with the same hex digit length that
     * contains the position.  Numbers with len hex digits range from
     * 16^(len-1) to 16^len - 1, exactly 15·16^(len-1) numbers. */
    while (1) {
        idx_t count           = 15 * pow16;          /* #numbers in this interval */
        idx_t digits_interval = count * len;         /* total hex digits */
        if (pos <= digits_interval) {
            break;
        }
        pos -= digits_interval;
        pow16 *= 16;
        ++len;
    }

    /* Position within the selected interval */
    idx_t offset       = pos - 1;               /* 0‑based */
    idx_t number_index = offset / len;          /* which number in interval */
    idx_t digit_index  = offset % len;          /* 0 = most significant digit */

    seq_t number = (seq_t)(pow16 + number_index);  /* actual decimal number */
    /* Shift to bring the desired hex digit to the lowest nibble */
    idx_t shift = (len - digit_index - 1) * 4;
    seq_t digit = (number >> shift) & 0xF;        /* extract hex digit */

    return digit;
}
