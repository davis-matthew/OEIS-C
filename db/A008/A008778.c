/*
 * A008778  a(n) = (n+1)*(n^2 +8*n +6)/6. Number of n-dimensional partitions
 *          of 4. Number of terms in 4th derivative of a function composed
 *          with itself n times.
 * https://oeis.org/A008778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^2 + 8*n + 6)/6
 * Range:     u8 n <= 3, u16 n <= 37, u32 n <= 2949, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;           /* cast index to seq_t for exact arithmetic */
    seq_t one = N + 1;              /* (n+1) */
    seq_t sq  = N * N;              /* n^2 */
    sq += 8 * N;                    /* n^2 + 8*n */
    sq += 6;                        /* n^2 + 8*n + 6 */
    seq_t num = one * sq;           /* numerator (n+1)*(n^2+8*n+6) */
    return num / 6;                 /* integer division – always exact */
}
