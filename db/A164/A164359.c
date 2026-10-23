/*
 * A164359  Expansion of (1 - x^2)^3 / ((1 - x)^3 * (1 - x^3)) in powers of x.
 * https://oeis.org/A164359
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n > 0 : a(n) = 2 if n mod 3 == 0, otherwise a(n) =
 *            3
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)                /* a(0) = 1 */
        return (seq_t)1;
    if (n % 3 == 0)            /* a(3k) = 2 for k > 0 */
        return (seq_t)2;
    return (seq_t)3;           /* a(3k+1) = a(3k+2) = 3 */
}
