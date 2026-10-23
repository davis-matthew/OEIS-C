/*
 * A124355  Number of (directed) Hamiltonian cycles on the complete graph K_n.
 * https://oeis.org/A124355
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(n) = (n-1)! for n > 2
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return (seq_t)0;

    seq_t res = 1;
    for (idx_t i = 2; i < n; ++i)
        res *= (seq_t)i;

    return res;
}
