/*
 * A024401  a(n) = [ (3rd elementary symmetric function of S(n))/(2nd
 *          elementary symmetric function of S(n)) ], where S(n) = {first n+2
 *          positive integers congruent to 2 mod 3}.
 * https://oeis.org/A024401
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((3*n^2 + 5*n - 2)/6) (offset 1)
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t for arithmetic on unsigned integer type */
    seq_t N = (seq_t)n;
    /* Compute numerator using unsigned arithmetic (3*N*N + 5*N - 2) */
    seq_t numerator = 3 * N * N + 5 * N - (seq_t)2;
    /* Integer division truncates toward zero; for n ≥ 1 this yields the floor */
    return numerator / (seq_t)6;
}
