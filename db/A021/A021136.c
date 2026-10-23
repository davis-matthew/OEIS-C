/*
 * A021136  Decimal expansion of 1/132.
 * https://oeis.org/A021136
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = n < 2 ? 0 : (n % 2 == 0) ? 7 : 5
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The decimal expansion of 1/132 is 0.0075757575…, so the first two
 *  digits are 0,0 and thereafter the digits alternate 7,5. */
seq_t a(idx_t n)
{
    if (n < 2) {
        return 0;
    }
    /* for n >= 2, digit is 7 if n is even, 5 if n is odd */
    return (n & 1) ? 5 : 7;
}
