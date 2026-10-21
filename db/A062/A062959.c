/*
 * A062959  Number of divisors of n^(n!) (A053986).
 * https://oeis.org/A062959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n^(n!)) (number of divisors of n^(n!))
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 9, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* factor n and accumulate the divisor count */
    idx_t m = n;
    seq_t ans = 1;

    for (idx_t p = 2; p * p <= m; ++p) {
        if (m % p == 0) {
            idx_t exp = 0;
            while (m % p == 0) {
                m /= p;
                ++exp;
            }
            ans *= (fact * (seq_t)exp + (seq_t)1);
        }
    }

    /* if m is now a prime > 1 */
    if (m > 1) {
        ans *= (fact + (seq_t)1);
    }

    return ans;
}
