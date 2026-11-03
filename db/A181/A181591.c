/*
 * A181591  a(n) = binomial(bigOmega(n),omega(n)), where omega = A001221 and
 *          bigOmega = A001222.
 * https://oeis.org/A181591
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(bigOmega(n), omega(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 116 terms through n = 9768 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* bigOmega(n) = total number of prime factors of n with multiplicity (A001222) */
/* omega(n)    = number of distinct prime factors of n          (A001221) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t bigomega(idx_t n)
{
    /* Count prime factors of n with multiplicity. */
    if (n <= 1)
        return 0;

    seq_t count = 0;

    /* Count factors of 2. */
    while ((n & 1) == 0) {
        n >>= 1;
        ++count;
    }

    /* Test odd divisors up to sqrt(n). */
    for (idx_t p = 3; p * p <= n; p += 2) {
        while (n % p == 0) {
            n /= p;
            ++count;
        }
    }

    /* If n > 1 now, it is a prime factor > sqrt(original n). */
    if (n > 1)
        ++count;

    return count;
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

/* Compute binomial coefficient C(a,b) using multiplicative formula. */
static seq_t binom(idx_t a, idx_t b) {
    if (b > a) return 0;
    if (b > a - b) b = a - b;          /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= b; ++i) {
        res *= (seq_t)(a - b + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1; for n = 0 return 0. */
    if (n == 0)
        return (seq_t)0;

    /* bigOmega is the total number of prime factors with multiplicity. */
    seq_t bigOmega = bigomega(n);

    /* omega is the number of distinct prime factors. */
    int omega = count_distinct_primes((uint64_t)n);

    /* Compute binomial(bigOmega, omega). The binom routine expects idx_t
       arguments; cast appropriately.  For typical n < 2^64 the conversion
       is safe. */
    return binom((idx_t)bigOmega, (idx_t)omega);
}
