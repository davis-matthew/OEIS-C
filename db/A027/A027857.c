/*
 * A027857  Number of positive divisors of n!, read mod n.
 * https://oeis.org/A027857
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of positive divisors of n! modulo n
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined for n = 0, just return 0 */

    /* Handle trivial case n = 1 */
    if (n == 1)
        return 0;            /* d(1!) = 1, 1 mod 1 = 0 */

    /* Sieve up to n to get all primes <= n */
    size_t sz = (size_t)n + 1;
    char *is_prime = (char *)malloc(sz);
    if (!is_prime)
        return 0;            /* memory allocation failure */

    /* Initialize all entries to prime (1) */
    for (size_t i = 0; i < sz; ++i)
        is_prime[i] = 1;
    is_prime[0] = is_prime[1] = 0;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (is_prime[p]) {
            for (idx_t m = p * p; m <= n; m += p)
                is_prime[m] = 0;
        }
    }

    /* Compute d(n!) modulo n */
    uint64_t mod = (uint64_t)n;
    uint64_t result = 1ULL;

    for (idx_t p = 2; p <= n; ++p) {
        if (!is_prime[p])
            continue;

        /* exponent of prime p in n! */
        idx_t t = n / p;
        idx_t e = 0;
        while (t) {
            e += t;
            t /= p;
        }

        /* multiply (e + 1) into the result modulo n */
        result = (result * (uint64_t)(e + 1)) % mod;
    }

    /* Ensure result is reduced modulo n for n = 1 case */
    result %= mod;

    free(is_prime);
    return (seq_t)result;
}
