/*
 * A000498  Eulerian numbers (Euler's triangle: column k=4 of A008292, column
 *          k=3 of A173018).
 * https://oeis.org/A000498
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 4^n - (n+1)*3^n + (n*(n+1)/2)*2^n - (n-1)*n*(n+1)/6
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_seq.h"

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t tri = ns * (ns + 1) / 2;        /* n(n+1)/2 */
    seq_t tet = tri * (ns - 1) / 3;       /* (n-1)n(n+1)/6 */

    seq_t pos = pow_seq(4, n) + tri * pow_seq(2, n);
    seq_t neg = (ns + 1) * pow_seq(3, n) + tet;
    return pos - neg;
}
