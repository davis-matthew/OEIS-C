/*
 * A096509  Number of prime-powers [including primes] in the (up and down)
 *          neighborhood of n with Ceiling[Log[n]] radius.
 * https://oeis.org/A096509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of prime‑powers (including primes) in the interval
 *            [ n – ceil(log_e n) , n + ceil(log_e n) ]. The implementation
 *            uses only integer arithmetic, float only for computing
 *            ceil(log_e n). It works for any idx_t / seq_t types provided by
 *            the build
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer ceiling of natural logarithm (base e) */
static idx_t ceil_log_e(idx_t n)
{
    if (n <= 1) return 0;
    double d = (double)n;
    const double e = 2.71828182845904523536;
    idx_t r = 0;
    while (d > 1.0) {
        d /= e;
        ++r;
    }
    return r;
}

/* primality test for small n (<= √max(x)) */
static int isPrime(idx_t n)
{
    if (n < 2)                 return 0;
    if (n == 2 || n == 3)      return 1;
    if ((n & 1U) == 0)         return 0;
    for (idx_t i = 3; i * i <= n; i += 2)
        if (n % i == 0)        return 0;
    return 1;
}

/* check whether x ≥ 2 is a prime power (p^k, k ≥ 1 with p prime) */
static int isPrimePower(idx_t x)
{
    if (x < 2) return 0;

    /* First check if x itself is prime. */
    if (isPrime(x)) return 1;

    /* Find the smallest prime factor of x. */
    idx_t p = 0;
    for (idx_t f = 2; f * f <= x; ++f) {
        if (x % f == 0) {
            if (isPrime(f)) {
                p = f;
                break;
            }
            /* if f is not prime, the factorisation will involve
               more than one distinct prime factor: not a prime power. */
            return 0;
        }
    }
    if (p == 0) return 0;   /* should not happen (x not prime, but no prime factor found) */

    /* Divide out all powers of p. */
    while (x % p == 0)
        x /= p;

    /* If anything remains, x had a second distinct prime factor. */
    return x == 1;
}

/* OEIS sequence A096509 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety; sequence starts at n = 1 */

    idx_t r      = ceil_log_e(n);
    idx_t start  = (n > r) ? (n - r) : 1;
    idx_t end    = n + r;

    seq_t count = 0;
    for (idx_t x = start; x <= end; ++x) {
        if (isPrimePower(x))
            ++count;
    }
    return count;
}
