/*
 * A205745  a(n) = card { d | d*p = n, d odd, p prime }.
 * https://oeis.org/A205745
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime divisors p of n such that n/p is odd.
 *            Equivalent: for odd n, count distinct odd prime factors of n;
 *            for even n, the answer is 1 if n is divisible by 2 exactly once,
 *            else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Undefined behaviour for n == 0 is harmless; return 0 */
    if (n == 0) return (seq_t)0;

    /* If n is even, only prime 2 can potentially satisfy n/p odd.
       Count the exponent of 2 in n. */
    if ((n & 1ULL) == 0ULL) {
        idx_t m = n;
        int exp2 = 0;
        while ((m & 1ULL) == 0ULL) {
            m >>= 1;
            ++exp2;
        }
        return exp2 == 1 ? (seq_t)1 : (seq_t)0;
    }

    /* n is odd: count distinct odd prime divisors. */
    idx_t tmp = n;
    seq_t cnt = 0;
    /* Test divisors 3,5,7,... up to sqrt(tmp) */
    for (idx_t p = 3; p <= tmp / p; p += 2) {
        if (tmp % p == 0ULL) {
            ++cnt;
            while (tmp % p == 0ULL) {
                tmp /= p;
            }
        }
    }
    /* If after division tmp > 1, it is a remaining prime factor. */
    if (tmp > 1ULL) {
        ++cnt;
    }
    return cnt;
}
