/*
 * A021270  Decimal expansion of 1/266.
 * https://oeis.org/A021270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1) / 266) % 10 Computes the nth decimal digit
 *            of 1/266 by repeated division
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use 64‑bit idx_t for intermediate calculations to avoid overflow
       when the build uses a small seq_t. */
    idx_t rem   = 1; /* remainder before generating the next digit */
    idx_t digit = 0;
    const idx_t divisor = 266;

    for (idx_t i = 0; i <= n; ++i) {
        rem   *= 10;
        digit  = rem / divisor;
        rem    = rem % divisor;
    }
    return (seq_t)digit;
}
