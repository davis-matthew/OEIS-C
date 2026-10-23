/*
 * A046110  Number of lattice points on circumference of a circle of radius
 *          (2n+1)/2 with center at (1/2,0).
 * https://oeis.org/A046110
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 * A046080(2*n+1) + 2 where A046080(m) = sum of 2^{k-1}
 *            over all positive divisors t of m with k = number of distinct
 *            primes ≡ 1 (mod 4) dividing t. This counts unordered positive
 *            integer solutions (u,v) with u odd, v even to u² + v² = m². The
 *            term +2 accounts for the two points with v = 0
 * Range:     u8 n <= 97, u16 n <= 97, u32 n <= 97, u64 n <= 97, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;          /* m is always odd */
    idx_t temp = m;
    /* store exponents of primes p ≡ 1 mod 4 */
    int exps[20];
    int t = 0;                    /* number of such primes */

    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            if (p % 4 == 1) {
                exps[t++] = exp;
            }
            /* primes ≡ 3 mod 4 do not contribute to A046080 */
        }
    }
    if (temp > 1) {               /* remaining prime factor */
        if (temp % 4 == 1) {
            exps[t++] = 1;
        }
    }

    /* dynamic programming over subsets of the primes ≡1 (mod4) */
    seq_t dp[25];
    for (int i = 0; i <= t; ++i) dp[i] = 0;
    dp[0] = 1;                    /* product of zero primes is 1 */

    for (int i = 0; i < t; ++i) {
        int a = exps[i];
        for (int k = i; k >= 0; --k) {
            dp[k + 1] += dp[k] * a;
        }
    }

    seq_t A = 0;
    seq_t pow2 = 1;                /* 2^(k-1) for k=1 */
    for (int k = 1; k <= t; ++k) {
        A += dp[k] * pow2;
        pow2 <<= 1;                /* multiply by 2 for next k */
    }

    return 4 * A + 2;
}
