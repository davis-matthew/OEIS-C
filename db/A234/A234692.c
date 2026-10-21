/*
 * A234692  Decimal value of the bitmap of active segments in 7-segment
 *          display of the number n, variant 2 ("abcdefg" scheme: bits
 *          represent segments in clockwise order).
 * https://oeis.org/A234692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n mod 10) + a(floor(n/10))*2^7 Initial values: a(0)=63,
 *            a(1)=6, a(2)=91, a(3)=79, a(4)=102, a(5)=109, a(6)=125, a(7)=39,
 *            a(8)=127, a(9)=111
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, if needed */

seq_t a(idx_t n)
{
    /* 7‑segment encoding for decimal digits 0–9 in the "abcdefg" scheme */
    static const seq_t seg[10] = {
        (seq_t)63,  /* 0 */
        (seq_t)6,   /* 1 */
        (seq_t)91,  /* 2 */
        (seq_t)79,  /* 3 */
        (seq_t)102, /* 4 */
        (seq_t)109, /* 5 */
        (seq_t)125, /* 6 */
        (seq_t)39,  /* 7 */
        (seq_t)127, /* 8 */
        (seq_t)111  /* 9 */
    };

    if (n == 0)
        return seg[0];

    seq_t result = 0;
    seq_t mult   = 1;      /* 2^0 initially, will shift left by 7 bits per digit */

    while (n != 0) {
        idx_t digit = n % 10;
        result += seg[digit] * mult;
        mult <<= 7;          /* multiply by 2^7 for the next digit position */
        n /= 10;
    }

    return result;
}
