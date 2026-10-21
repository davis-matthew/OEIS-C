/*
 * A223571  Number of nX3 0..2 arrays with antidiagonals unimodal.
 * https://oeis.org/A223571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=27, a(2)=729, a(n)=22*a(n-1) for n>2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)27;
    if (n == 2) return (seq_t)729;
    seq_t res = (seq_t)729;
    for (idx_t i = 3; i <= n; ++i)
        res = res * (seq_t)22;
    return res;
}
