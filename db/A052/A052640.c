/*
 * A052640  E.g.f. x*(1-x)/(1-2*x-x^2+x^3).
 * https://oeis.org/A052640
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=2,
   (n^3+6n^2+11n+6)*a(n) + (-n^2-5n-6)*a(n+1)
   + (-2n-6)*a(n+2) + a(n+3) = 0            */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* Sliding window of the last three values */
    seq_t a0 = (seq_t)0;   /* a(i)   */
    seq_t a1 = (seq_t)1;   /* a(i+1) */
    seq_t a2 = (seq_t)2;   /* a(i+2) */

    for (idx_t i = 0; i + 3 <= n; ++i)
    {
        /* Integer coefficients for the current step */
        unsigned long long ii   = (unsigned long long)i;
        unsigned long long c0   = ii*ii*ii + 6ULL*ii*ii + 11ULL*ii + 6ULL;   /* n^3+6n^2+11n+6 */
        unsigned long long c1   = ii*ii     + 5ULL*ii     + 6ULL;           /* n^2+5n+6   */
        unsigned long long c2   = 2ULL*ii      + 6ULL;                      /* 2n+6       */

        seq_t term1 = a1 * (seq_t)c1;
        seq_t term2 = a2 * (seq_t)c2;
        seq_t term0 = a0 * (seq_t)c0;

        seq_t next = term1 + term2 - term0;       /* a(i+3) */

        /* Shift window for the next iteration */
        a0 = a1;
        a1 = a2;
        a2 = next;
    }

    return a2;   /* a(n) is now stored in a2 after the last iteration */
}
