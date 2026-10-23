/*
 * A016851  a(n) = (5*n)^3.
 * https://oeis.org/A016851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n)^3
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 325, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t to avoid premature overflow on smaller types. */
    seq_t t = (seq_t)5 * (seq_t)n;   /* t = 5*n */
    return t * t * t;                /* a(n) = t^3 = (5*n)^3 */
}
