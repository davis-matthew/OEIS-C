/*
 * A189458  a(n) = n+[nr/s]+[nt/s]; r=2, s=sqrt(2), t=1+sqrt(2).
 * https://oeis.org/A189458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + floor(n*sqrt(2)) + floor(n*sqrt(2)/2)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Rational approximation of sqrt(2) with 18 decimal places:
       sqrt(2) ≈ 1414213562373095048 / 10^18   */
    const seq_t P  = 1414213562373095048ULL;           /* floor( sqrt(2) * 10^18 ) */
    const seq_t Q  = 1000000000000000000ULL;           /* 10^18 */
    const seq_t P2 = 707106781186547524ULL;           /* floor( (sqrt(2)/2) * 10^18 ) */

    seq_t ns = (seq_t)n;

    /* floor(n * sqrt(2))  = floor( n * P / Q )  */
    seq_t f1 = (seq_t)((__int128)ns * P / Q);
    /* floor(n * sqrt(2)/2) = floor( n * P2 / Q ) */
    seq_t f2 = (seq_t)((__int128)ns * P2 / Q);

    return 2 * ns + f1 + f2;
}
