/*
 * A038758  Number of ways of covering a 2n X 2n lattice by 2n^2 dominoes with
 *          exactly 4 horizontal (or vertical) dominoes.
 * https://oeis.org/A038758
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/24)*n*(n-1)*(n+1)*(12*n^3-11*n-10)
 * Range:     u8 none, u16 n <= 4, u32 n <= 26, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 2  (a(2) is the first listed term)              */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 2.  For n < 2 we simply return 0. */
    if (n < 2) return 0;

    /* Cast the index to the arbitrary‑precision type. */
    seq_t N   = (seq_t)n;
    seq_t Nm1 = N - 1;    /* N-1 */
    seq_t Np1 = N + 1;    /* N+1 */

    /* Compute N³ and the polynomial 12*N³ – 11*N – 10. */
    seq_t N2   = N * N;           /* N²   */
    seq_t N3   = N2 * N;          /* N³   */
    seq_t poly = 12 * N3 - 11 * N - 10;  /* 12*N³ – 11*N – 10 */

    /* Multiply all factors and divide by 24 (division is exact). */
    seq_t prod = N * Nm1;
    prod = prod * Np1;
    prod = prod * poly;
    seq_t result = prod / 24;

    return result;
}
