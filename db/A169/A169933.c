/*
 * A169933  a(n) = 2+n in the arithmetic defined in A169918.
 * https://oeis.org/A169933
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n - (n % 10) + (2 * (n % 10) % 10)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent to: replace the last decimal digit d of n by (2*d mod 10) */
seq_t a(idx_t n)
{
    idx_t d   = n % 10u;          /* last decimal digit */
    idx_t nd  = (d * 2u) % 10u;   /* doubled modulo 10 */
    return (seq_t)(n / 10u) * 10u + nd;
}
