/*
 * A158210  a(n) = omega(n) * (-1)^mu(n), where mu is the Moebius function.
 * https://oeis.org/A158210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = A001221(n) * (-1)^A008683(n)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 97 terms through n = 9964 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t mobius(idx_t m)
{
    if (m == 0) return 0;          /* defined as 0, but 0 is never passed */
    seq_t sign = 1;                /* product of (–1) for each prime factor */
    idx_t tmp = m;
    for (idx_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            int cnt = 0;
            while (tmp % p == 0)
            {
                tmp /= p;
                ++cnt;
                if (cnt > 1)
                    return 0;     /* not square‑free */
            }
            sign = -sign;
        }
    }
    if (tmp > 1)                   /* remaining prime factor */
        sign = -sign;
    return sign;
}

/* Count distinct prime factors of an unsigned 64-bit integer n.
   The result fits in a signed int for all n that may be passed. */
static int count_distinct_primes(uint64_t n)
{
    if (n <= 1)
        return 0;

    int cnt = 0;
    /* Handle factor 2 separately. */
    if (n % 2 == 0) {
        cnt++;
        while (n % 2 == 0)
            n /= 2;
    }
    /* Test odd divisors up to sqrt(n). */
    for (uint64_t p = 3; p <= n / p; p += 2) {
        if (n % p == 0) {
            cnt++;
            while (n % p == 0)
                n /= p;
        }
    }
    if (n > 1)   /* Remaining prime factor. */
        cnt++;
    return cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                           /* undefined, return 0 for safety */

    /* number of distinct prime factors of n (A001221) */
    seq_t omega = (seq_t)count_distinct_primes((uint64_t)n);

    /* Möbius function μ(n) (A008683): 0, 1, or -1 */
    seq_t mu = mobius((idx_t)n);

    /* compute (-1)^μ(n): +1 if μ(n)=0, otherwise -1 */
    seq_t sign = (mu == 0) ? (seq_t)1 : (seq_t)-1;

    return omega * sign;
}
