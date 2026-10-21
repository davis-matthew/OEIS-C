/*
 * A090223  Nonnegative integers with doubled multiples of 4.
 * https://oeis.org/A090223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(4*n/5)
 * Range:     u8 n <= 318, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Avoid overflow on 64‑bit builds by performing the division first */
    seq_t q = (seq_t)n / 5;          /* quotient of n/5 */
    seq_t r = (seq_t)n % 5;          /* remainder (0..4) */
    return q * 4 + (r * 4) / 5;      /* floor(4*n/5) */
}
