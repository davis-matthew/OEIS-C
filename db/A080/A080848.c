/*
 * A080848  a(n) = n*(mu(n)+2), where mu is the Moebius function (A008683).
 * https://oeis.org/A080848
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(mu(n)+2)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the Möbius function μ(n) by trial division. */
    int mu = 1;          /* μ(1) = 1 */
    idx_t t = n;

    for (idx_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            int cnt = 0;
            while (t % p == 0) {
                t /= p;
                ++cnt;
                if (cnt > 1) {     /* a squared prime factor -> μ = 0 */
                    mu = 0;
                    /* terminate the outer loop immediately */
                    p = t;
                    break;
                }
            }
            if (mu != 0)           /* a new distinct prime factor */
                mu = -mu;
        }
    }

    if (mu != 0 && t > 1)          /* final prime factor, if any */
        mu = -mu;

    return (seq_t)n * (seq_t)(mu + 2);
}
