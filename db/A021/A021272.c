/*
 * A021272  Decimal expansion of 1/268.
 * https://oeis.org/A021272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: digits of decimal expansion of 1/268 after the decimal point */
seq_t a(idx_t n)
{
    const idx_t denom = 268;
    idx_t rem = 1;          /* initial remainder after decimal point */
    idx_t i   = 0;          /* current digit index */
    while (1) {
        rem *= 10;
        idx_t digit = rem / denom;
        rem = rem % denom;
        if (i == n) return (seq_t)digit;
        ++i;
    }
    return 0;                /* never reached, suppresses warnings */
}
