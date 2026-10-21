/*
 * A222410  Partial sums of A008534, or crystal ball sequence for {A_6}*
 *          lattice.
 * https://oeis.org/A222410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (7/36) * n * (n+1) * (n^4 + 2*n^3 + 8*n^2 + 7*n + 18)
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 51, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast the index to the sequence type to avoid accidental use of 64‑bit
       arithmetic with potential overflow. */
    seq_t N  = (seq_t)n;
    seq_t N1 = N + 1;

    /* intermediate powers of n */
    seq_t n2 = N * N;          /* n^2   */
    seq_t n3 = n2 * N;         /* n^3   */
    seq_t n4 = n3 * N;         /* n^4   */

    /* the polynomial part: n^4 + 2*n^3 + 8*n^2 + 7*n + 18 */
    seq_t poly = n4
               + (seq_t)2 * n3
               + (seq_t)8 * n2
               + (seq_t)7 * N
               + (seq_t)18;

    /* product n*(n+1)*poly */
    seq_t prod = N * N1;
    prod *= poly;

    /* multiply by 7 and divide by 36 (exact division) */
    prod *= (seq_t)7;
    prod /= (seq_t)36;

    /* add the constant 1 */
    return (seq_t)1 + prod;
}
