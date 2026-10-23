/*
 * A168284  a(n) = (10*n + 5*(-1)^n - 5)/4.
 * https://oeis.org/A168284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 5*floor(n/2) (equivalent to (10*n + 5*(-1)^n - 5)/4)
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;           // convert index to seq_t
    seq_t half   = n_seq / (seq_t)2;  // floor division by 2
    return half * (seq_t)5;           // multiply by 5
}
