/*
 * A069939  a(n) = (1/3!)*((Sum of digits of n)^3 + 3*(Sum of digits of
 *          n)*(Sum of digits^2 of n) + 2*(Sum of digits^3 of n)).
 * https://oeis.org/A069939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = (S1^3 + 3*S1*S2 + 2*S3) / 6
 * where S1 = Σ(di), S2 = Σ(di^2), S3 = Σ(di^3) for the decimal digits di of n.
 * This equals the sum of all products d_i*d_j*d_k with 1 ≤ i ≤ j ≤ k ≤ m.
 */
seq_t a(idx_t n)
{
    seq_t s1 = (seq_t)0; /* sum of digits */
    seq_t s2 = (seq_t)0; /* sum of squares of digits */
    seq_t s3 = (seq_t)0; /* sum of cubes of digits */
    idx_t m = n;

    while (m) {               /* extract digits of n */
        seq_t d = (seq_t)(m % (idx_t)10);
        s1 += d;
        s2 += d * d;
        s3 += d * d * d;
        m /= (idx_t)10;
    }

    /* Compute the numerator of the formula */
    seq_t numerator = s1 * s1 * s1 + 3 * s1 * s2 + 2 * s3;

    return numerator / (seq_t)6;
}
