/*
 * A050510  Golden rectangular box numbers: a(n) =
 *          n*A007067(n)*A007067(A007067(n)).
 * https://oeis.org/A050510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n * round(n*phi) * round(round(n*phi)*phi)
 * Range:     u8 n <= 1, u16 n <= 0, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where phi = (1+sqrt(5))/2; round(...) is nearest integer. */
seq_t a(idx_t n)
{
    /* Use an integer approximation of sqrt(5) with sufficient precision.
       Let K = 20000, S = floor(sqrt(5)*K) = 44721.  Then
          round(n*phi) = floor((n*K + n*S + K) / (2*K)).           */
    const seq_t K = 20000;
    const seq_t S = 44721;               /* floor(sqrt(5) * K) */

    /* First round */
    seq_t num1 = (seq_t)n * K + (seq_t)n * S + K;
    seq_t r1   = num1 / (K * 2);

    /* Second round */
    seq_t num2 = r1 * K + r1 * S + K;
    seq_t r2   = num2 / (K * 2);

    /* Final product */
    return (seq_t)n * r1 * r2;
}
