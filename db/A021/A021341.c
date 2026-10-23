/*
 * A021341  Decimal expansion of 1/337.
 * https://oeis.org/A021341
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = digit n of the decimal expansion of 1/337 (a(0) is the
 *            first digit after the decimal point)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Simple long‑division to extract the n‑th digit. */
    unsigned rem = 1u;  /* remainder before decimal point */
    unsigned digit = 0u;

    for (idx_t i = 0; i <= n; ++i) {
        rem *= 10u;                /* shift remainder left by one decimal place */
        digit = rem / 337u;        /* integer part of division gives the digit */
        rem  %= 337u;              /* new remainder for next step */
    }

    return (seq_t)digit;
}
