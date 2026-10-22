/*
 * A005905  Number of points on surface of truncated tetrahedron: a(n) =
 *          14*n^2 + 2 for n > 0, a(0)=1.
 * https://oeis.org/A005905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 14*n^2 + 2 for n > 0
 * Range:     u8 n <= 4, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    
    seq_t n_seq = (seq_t)n;
    seq_t n_sq = n_seq * n_seq;
    return 14 * n_sq + 2;
}
