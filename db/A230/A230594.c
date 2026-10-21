/*
 * A230594  Number of ways to write n as n = x*y, where x, y = noncomposite
 *          numbers (A008578), 1 <= x <= n, 1 <= y <= n.
 * https://oeis.org/A230594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} A080339(d) * A080339(n/d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A080339(d) = 1 if d is 1 or prime, otherwise 0. */

seq_t a(idx_t n)
{
    /* Handle the special case n = 1 */
    if (n == 1) {
        return (seq_t)1;
    }

    idx_t temp = n;
    int tot = 0;        /* total number of prime factors (with multiplicity) */
    int sq = 0;         /* flag: 1 if a prime factor with exponent > 1 was found */

    /* trial division to factor n */
    for (idx_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            int cnt = 0;
            while (temp % d == 0) {
                temp /= d;
                ++cnt;
            }
            tot += cnt;
            if (cnt > 1) {
                sq = 1;
            }
            /* if more than two prime factors, a(n) is 0 */
            if (tot > 2) {
                return (seq_t)0;
            }
        }
    }

    /* if temp > 1, it's a remaining prime factor */
    if (temp > 1) {
        ++tot;
    }

    /* determine the value according to the number of prime factors */
    if (tot == 1) {
        /* n is prime */
        return (seq_t)2;
    } else if (tot == 2) {
        if (sq) {
            /* n = p^2 */
            return (seq_t)1;
        } else {
            /* n = p * q, p != q */
            return (seq_t)2;
        }
    } else {
        /* n has 0 or >2 prime factors (or composite non‑square) */
        return (seq_t)0;
    }
}
