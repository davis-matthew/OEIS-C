/*
 * A178734  a(n) = n XOR 8n, where XOR is bitwise XOR.
 * https://oeis.org/A178734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n XOR 8n
 * Range:     u8 n <= 31, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast to seq_t before performing operations */
    seq_t sn   = (seq_t)n;     /* n as seq_t */
    seq_t prod = sn << 3;      /* 8*n */
    return sn ^ prod;          /* n XOR (8*n) */
}
