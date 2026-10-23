/*
 * A021637  Decimal expansion of 1/633.
 * https://oeis.org/A021637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10^(n+1)/633) mod 10
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th digit after the decimal point of 1/633 by long division. */
seq_t a(idx_t n)
{
    idx_t remainder = 1;   /* starting remainder (1 before the first decimal digit) */
    idx_t digit = 0;       /* current digit (0–9) */

    for (idx_t i = 0; i <= n; ++i) {
        remainder *= 10u;
        digit = remainder / 633u;
        remainder -= digit * 633u;
    }
    return (seq_t)digit;
}
