/*
 * A057569  Numbers of the form k*(5*k+1)/2 or k*(5*k-1)/2.
 * https://oeis.org/A057569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n-2)/8 if n even, a(n) = (5*n-3)*(n-1)/8 if n odd
 * Range:     u8 n <= 21, u16 n <= 324, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1ULL) == 0) {                      /* even n */
        seq_t t = (seq_t)5 * N - (seq_t)2;
        /* Use half of n to keep intermediate smaller: a = (n/2)*t/4 */
        seq_t half = N >> 1;                     /* N/2 */
        return (half * t) / (seq_t)4;
    } else {                                    /* odd n */
        seq_t t = (seq_t)5 * N - (seq_t)3;       /* 5*n-3 */
        seq_t half = (N - (seq_t)1) >> 1;       /* (n-1)/2 */
        return (t * half) / (seq_t)4;
    }
}
