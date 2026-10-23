/*
 * A158865  Smallest maximal excess attained by an equivalence class of
 *          Hadamard matrices of order 4n.
 * https://oeis.org/A158865
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  hard, more, nonn
 * Formula:   a(n) = 2*n*(n+3)
 * Range:     u8 n <= 7, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t k = (seq_t)n;
    return k * (k + 3) * 2;
}
