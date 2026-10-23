/*
 * A137883  Number of (directed) Hamiltonian paths in the n-Möbius ladder
 *          graph.
 * https://oeis.org/A137883
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n*(n*n + (n%2==0 ? 2 : 3))
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t extra = (n & 1) ? 3 : 2;
    seq_t inner = nn + extra;
    return 2 * (seq_t)n * inner;
}
