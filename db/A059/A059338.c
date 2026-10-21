/*
 * A059338  a(n) = Sum_{k=1..n} k^5 * binomial(n,k).
 * https://oeis.org/A059338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-5) * n^2 * (n^3 + 10*n^2 + 15*n - 10)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 16, u64 n <= 41, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For indices below the defined offset the value is unspecified,
       so we simply return 0. */
    if (n == 0)
        return (seq_t)0;

    /* Work entirely in seq_t to preserve arbitrary precision where
       applicable.  All constants are cast to seq_t before use. */
    seq_t sn  = (seq_t)n;                 /* n */
    seq_t s2  = sn * sn;                  /* n^2 */
    seq_t s3  = s2 * sn;                  /* n^3 */

    seq_t inner = s3
                  + (seq_t)10 * s2      /* +10*n^2 */
                  + (seq_t)15 * sn      /* +15*n   */
                  - (seq_t)10;         /* -10     */

    seq_t product = s2 * inner;           /* n^2 * (n^3 + 10*n^2 + 15*n - 10) */

    /* Multiply by 2^(n-5) for n >= 5, otherwise shift right to cancel
       the corresponding power of two.  The product is always divisible
       by the needed power of two, so shifting produces an exact
       integer result. */
    if (n >= 5)
        return product << (n - 5);
    else
        return product >> (5 - n);
}
