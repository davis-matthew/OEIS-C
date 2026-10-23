/*
 * A021793  Decimal expansion of 1/789.
 * https://oeis.org/A021793
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A021793: Decimal expansion of 1/789.
 * a(n) is the n‑th digit after the decimal point in 0.001267427122940430...
 * Formula: a(n) = floor(10^(n+1) / 789) mod 10
 */
seq_t a(idx_t n)
{
    const seq_t denom = 789;          /* denominator of the fraction */
    const seq_t mult  = 10;           /* multiplier for digit extraction */
    seq_t rem = 1;                    /* initial remainder 1/789 */
    seq_t digit = 0;
    for (idx_t i = 0; i <= n; ++i) {
        seq_t prod = rem * mult;      /* intermediate product (promoted) */
        digit = prod / denom;         /* current decimal digit */
        rem   = prod % denom;         /* remainder for the next step */
    }
    return digit;
}
