/*
 * A063956  Sum of the unitary prime divisors of n.
 * https://oeis.org/A063956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{p|n, p prime, gcd(p,n/p)=1} p (sum of unitary prime
 *            divisors)
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;          /* a(1) = 0 by definition */

    seq_t sum = 0;
    idx_t temp = n;

    /* Factor out the prime 2 */
    if ((temp & 1) == 0) {
        idx_t e = 0;
        while ((temp & 1) == 0) {
            temp >>= 1;   /* divide by 2 */
            e++;
        }
        if (e == 1) sum += 2;
    }

    /* Factor odd primes */
    for (idx_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            idx_t e = 0;
            while (temp % p == 0) {
                temp /= p;
                e++;
            }
            if (e == 1) sum += (seq_t)p;
        }
    }

    /* Remaining prime factor, if any */
    if (temp > 1) {
        /* its exponent is 1 by construction */
        sum += (seq_t)temp;
    }

    return sum;
}
