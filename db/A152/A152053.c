/*
 * A152053  a(n) = A144433(3n+1) + A144433(3n+2) + A144433(3n+3).
 * https://oeis.org/A152053
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1) * (45 + 9*(-1)^n)) / 2
 * Range:     u8 n <= 9, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is equivalent to a(n) = 45*(n+1)/2 + 9*(-1)^n*(n+1)/2 */
seq_t a(idx_t n)
{
    seq_t one_plus_n = (seq_t)n + 1;                 /* (n+1) */
    seq_t coeff = 45 + 9 * ( ( (n & 1) == 0 ) ? 1 : -1 ); /* 45 + 9*(-1)^n */
    return (one_plus_n * coeff) / 2;
}
