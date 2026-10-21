/*
 * A209595  Number of 4X4 0..n arrays with every element equal to a diagonal
 *          or antidiagonal reflection.
 * https://oeis.org/A209595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^8 * ((n+1)*(2*n+1))^2
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 35, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    // Convert index to seq_t
    seq_t kp1 = (seq_t)n + 1;          // (n+1)
    seq_t mul  = 1;                    // accumulator for (n+1)^8
    for (int i = 0; i < 8; ++i)
        mul *= kp1;                    // compute (n+1)^8

    seq_t t2 = ((seq_t)n) * 2 + 1;      // (2*n + 1)
    seq_t t3 = kp1 * t2;                // (n+1)*(2*n+1)
    seq_t t4 = t3 * t3;                 // ((n+1)*(2*n+1))^2

    return mul * t4;                    // (n+1)^8 * ((n+1)*(2*n+1))^2
}
