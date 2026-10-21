/*
 * A075908  Fifth column of triangle A075499.
 * https://oeis.org/A075908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n * S2(n+5,5), S2(k,m) = Stirling numbers of the second
 *            kind
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute S2(n+5,5) with a small DP table */
    idx_t target = n + 5;                     /* k in S2(k,5) */
    seq_t s[6] = {0};
    s[0] = 1;                                 /* S2(0,0)=1, other S2(0,m>0)=0 */
    for (idx_t i = 1; i <= target; ++i) {
        /* store previous row values */
        seq_t p0 = s[0], p1 = s[1], p2 = s[2], p3 = s[3], p4 = s[4], p5 = s[5];
        /* new row */
        s[0] = 0;
        s[1] = p1 + p0;                      /* 1*p1 + p0 */
        s[2] = (seq_t)2 * p2 + p1;           /* 2*p2 + p1 */
        s[3] = (seq_t)3 * p3 + p2;           /* 3*p3 + p2 */
        s[4] = (seq_t)4 * p4 + p3;           /* 4*p4 + p3 */
        s[5] = (seq_t)5 * p5 + p4;           /* 5*p5 + p4 */
    }
    seq_t s5 = s[5];                         /* S2(n+5,5) */

    /* Compute 4^n by binary exponentiation */
    seq_t pow4 = 1;
    seq_t base = 4;
    idx_t e = n;
    while (e) {
        if (e & 1ULL) pow4 *= base;
        base *= base;
        e >>= 1;
    }

    /* Final result a(n) = 4^n * S2(n+5,5) */
    return pow4 * s5;
}
