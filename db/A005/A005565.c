/*
 * A005565  Number of walks on square lattice.
 * https://oeis.org/A005565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = (1/4)*(n^4+14n^3+69n^2+136n+80)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 357, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    
    seq_t result = (n4 + 14*n3 + 69*n2 + 136*nn + 80) / 4;
    return result;
}
