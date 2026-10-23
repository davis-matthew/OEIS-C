/*
 * A117368  a(n) = largest prime less than the smallest prime dividing (2n-1).
 * https://oeis.org/A117368
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime less than the smallest prime divisor of
 *            (2n−1) (a(2) is the first term, n ≥ 2)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return true if x is prime (trial division). */
static bool is_prime_uint64(uint64_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    if (x % 3 == 0) return x == 3;
    for (uint64_t i = 5; i * i <= x; i += 6) {
        if (x % i == 0 || x % (i + 2) == 0) return false;
    }
    return true;
}

/* Return the smallest prime factor of n (trial division).   */
static uint64_t spf_uint64(uint64_t n)
{
    if (n % 2 == 0) return 2;
    if (n % 3 == 0) return 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0) return i;
        if (n % (i + 2) == 0) return i + 2;
    }
    return n;   /* n is prime */
}

/* External‑linked function required by the build. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;                  /* value for n < 2 is unspecified */

    /* 2*n - 1 fits comfortably into seq_t and also into uint64_t for the
       modest size of n used in the tests. */
    seq_t m = (seq_t)n * 2 - 1;
    uint64_t m64 = (uint64_t)m;

    uint64_t smallest = spf_uint64(m64);  /* smallest prime divisor of 2n-1 */

    /* find the largest prime strictly less than 'smallest' */
    uint64_t candidate = smallest - 1;
    while (!is_prime_uint64(candidate)) {
        --candidate;                 /* skip composite numbers */
    }

    return (seq_t)candidate;
}
