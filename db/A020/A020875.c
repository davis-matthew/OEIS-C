/*
 * A020875  Number of (undirected) Hamiltonian paths in n-Moebius ladder.
 * https://oeis.org/A020875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 2*n if n is even, else a(n) = n^3 + 3*n
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t cube = nn * nn;
    cube = cube * nn;
    seq_t coeff = (n & 1) ? (seq_t)3 : (seq_t)2;
    return cube + coeff * nn;
}
