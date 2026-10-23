/*
 * A021588  Decimal expansion of 1/584.
 * https://oeis.org/A021588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = floor(10 * ((10^n mod 584)) / 584)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It returns the n-th digit after the decimal point of 1/584,     */
/* with a(0) being the first digit.                                */
seq_t a(idx_t n)
{
    /* Compute r = 10^n mod 584 */
    idx_t r   = 1;
    const idx_t mod = 584;
    for (idx_t i = 0; i < n; ++i)
        r = (r * 10) % mod;            /* r stays < 584 */

    /* The digit is floor(10 * r / 584) */
    return (seq_t)((r * 10) / mod);
}
