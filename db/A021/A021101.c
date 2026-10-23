/*
 * A021101  Decimal expansion of 1/97.
 * https://oeis.org/A021101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the decimal digit at position n of the expansion
   of 1/97 after the decimal point.  a(0) is the first digit after the
   decimal point, a(1) the second, etc. */
seq_t a(idx_t n)
{
    /* remainder of the long division 1 / 97 */
    seq_t rem = 1;
    seq_t digit = 0;

    /* Compute digits up to position n (inclusive) */
    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10;
        digit = rem / 97;
        rem = rem % 97;
    }
    return digit;
}
