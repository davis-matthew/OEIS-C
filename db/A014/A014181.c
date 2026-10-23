/*
 * A014181  Numbers > 9 with all digits the same.
 * https://oeis.org/A014181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ((n-1)%9 + 1) * repunit( (n-1)/9 + 2 ), where repunit(m)
 *            = (10^m - 1)/9 (i.e., 111...1 with m digits)
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Determine length of the repdigit and the repeating digit. */
    const idx_t len = (n - 1) / 9 + 2;          /* number of digits   */
    const idx_t digit = (n - 1) % 9 + 1;        /* 1 .. 9            */

    /* Build repunit: 111..1 (len digits) */
    seq_t repunit = 0;
    for (idx_t i = 0; i < len; ++i) {
        repunit = repunit * (seq_t)10 + 1;
    }

    /* Multiply by the repeating digit to obtain the repdigit. */
    return repunit * (seq_t)digit;
}
