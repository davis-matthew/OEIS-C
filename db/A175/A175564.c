/*
 * A175564  Number of n X n binary matrices that contain no 2 X 2 submatrix
 *          consisting of all zeros or all ones.
 * https://oeis.org/A175564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=2, a(2)=14, a(3)=156, a(4)=840, a(n)=0 for n≥5
 * Range:     u8 n <= 3, u16 n <= 102, u32 n <= 102, u64 n <= 102,
 *            u128 n <= 102, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)2;
    if (n == 2u) return (seq_t)14;
    if (n == 3u) return (seq_t)156;
    if (n == 4u) return (seq_t)840;
    return (seq_t)0;
}
