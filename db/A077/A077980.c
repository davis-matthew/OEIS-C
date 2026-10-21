/*
 * A077980  Expansion of 1/(1 + x + 2*x^2 + 2*x^3).
 * https://oeis.org/A077980
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/3) * (-1)^⌊(n+1)/2⌋ * (2^⌊n/2⌋+1 + (-1)^⌊n/2⌋)
 * Range:     i8 n <= 15, i16 n <= 31, i32 n <= 63, i64 n <= 127,
 *            i128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via recurrence: let b(k) = a(2k). Then b(0)=1 and b(k)= -2*b(k-1)+1 for k≥1. */
/* Finally, a(2k)=b(k) and a(2k+1)= -b(k). */

seq_t a(idx_t n)
{
    /* k = floor(n/2)  */
    idx_t k = n >> 1;

    /* base value b(0) = a(0) = 1  */
    seq_t b = (seq_t)1;

    /* compute b(k) = -2*b(k-1)+1 iteratively   */
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t t = b * (seq_t)2;   /* t = 2 * b(k-1)  */
        b = -t + (seq_t)1;        /* b(k) = -2*b(k-1) + 1  */
    }

    /* final sign: a(2k)=b, a(2k+1)= -b  */
    return (n & (idx_t)1) ? -b : b;
}
