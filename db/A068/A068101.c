/*
 * A068101  a(n) = Sum_{k|n, k<=sqrt(n)} mu(k) where mu(k) is the Moebius
 *          function and the sum is over the positive divisors k of n with k
 *          <= sqrt(n).
 * https://oeis.org/A068101
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k|n, k <= sqrt(n)} mu(k)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, etc. */

/* Integer square root (floor) for uint64_t. */
static idx_t isqrt_u64(idx_t n)
{
    idx_t low   = 0;
    idx_t high  = n;
    idx_t result = 0;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        if (mid == 0) { /* avoid division by zero */
            low = mid + 1;
            continue;
        }
        if (mid <= n / mid) {           /* mid*mid <= n  */
            result = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return result;
}

/* Möbius function for 1 <= n. */
static int mobius(idx_t n)
{
    if (n == 1) return 1;
    int prime_factors = 0;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p) {
        if (m % p == 0) {
            int exp = 0;
            do {
                m /= p;
                exp++;
            } while (m % p == 0);
            if (exp > 1) return 0;   /* squared prime factor */
            prime_factors++;
        }
    }
    if (m > 1) prime_factors++;        /* remaining prime factor */
    return (prime_factors & 1) ? -1 : 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n=0, but harmless */
    seq_t sum = 0;
    idx_t lim = isqrt_u64(n);
    for (idx_t k = 1; k <= lim; ++k) {
        if (n % k == 0) {
            sum += (seq_t)mobius(k);
        }
    }
    return sum;
}
