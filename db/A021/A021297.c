/*
 * A021297  Decimal expansion of 1/293.
 * https://oeis.org/A021297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10 * ((10^n mod 293)) / 293)
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* computed by iterating the long‑division algorithm for 1/293 */

seq_t a(idx_t n)
{
    unsigned int rem = 1;                     /* remainder after 1/(denominator) */
    for (idx_t i = 0; i < n; ++i) {
        rem = (rem * 10u) % 293u;              /* next remainder */
    }
    unsigned int d = (rem * 10u) / 293u;       /* the nth decimal digit */
    return (seq_t)d;                           /* cast to seq_t */
}
