/*
 * A124349  Numbers of directed Hamiltonian cycles on the n-prism graph.
 * https://oeis.org/A124349
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + (1-(n mod 2))*4
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is assumed >= 3; behaviour for n < 3 is not defined. */
    seq_t sn = (seq_t)n;
    return sn * 2 + (1 - (sn & 1)) * 4;
}
