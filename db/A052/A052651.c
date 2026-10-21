/*
 * A052651  E.g.f. (1-x)/(1-x-x^3-x^4+x^5).
 * https://oeis.org/A052651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * g(n) where g(0)=1, g(1)=0, g(2)=0, g(3)=1, g(4)=2
 *            and for n >= 5: g(n) = g(n-1) + g(n-3) + g(n-4) - g(n-5)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* g values for the last up to five indices */
    seq_t g[5];
    g[0] = (seq_t)1;                /* g(0) */
    g[1] = (seq_t)0;                /* g(1) */
    g[2] = (seq_t)0;                /* g(2) */
    g[3] = (seq_t)1;                /* g(3) */
    g[4] = (seq_t)2;                /* g(4) */

    /* Handle small n directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)6;   /* 3! * 1 = 6 */
    if (n == 4) return (seq_t)48;  /* 4! * 2 = 48 */

    /* Compute g up to n */
    for (idx_t k = 5; k <= n; ++k) {
        /* indices modulo 5 */
        idx_t i1 = (k - 1) % 5;
        idx_t i3 = (k - 3) % 5;
        idx_t i4 = (k - 4) % 5;
        idx_t i5 = (k - 5) % 5;
        g[k % 5] = g[i1] + g[i3] + g[i4] - g[i5];
    }
    seq_t g_n = g[n % 5];

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    return fact * g_n;
}
