/*
 * A016780  a(n) = (3*n+1)^4.
 * https://oeis.org/A016780
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n+1)^4
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 84, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t before any multiplication to avoid
       overflow on the idx_t type. */
    seq_t t = ((seq_t)n * (seq_t)3) + (seq_t)1;
    seq_t t2 = t * t;
    return t2 * t2;
}
