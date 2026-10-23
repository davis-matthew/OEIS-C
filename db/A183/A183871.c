/*
 * A183871  a(n) = n + ceiling( (1/5)*n^2 ). Complement of A183870.
 * https://oeis.org/A183871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1)*(n+4)/5)
 * Range:     u8 n <= 33, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return ((seq_t)n + 1) * ((seq_t)n + 4) / 5;
}
