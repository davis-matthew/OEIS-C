/*
 * A140254  Mobius transform of A014963.
 * https://oeis.org/A140254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Σ_{d|n} μ(d) * b(n/d) where * b(m) = m if m is a prime
 *            power, * b(m) = 1 otherwise. (b(1)=1) * This yields the correct
 *            Möbius transform of A014963
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Validated: partial, pass, 77 terms through n = 77 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: factor n into its distinct prime factors.
 *  primes[] will contain the distinct primes, cnt will be the count. */
static void distinct_primes(idx_t n, int *primes, int *cnt)
{
    *cnt = 0;
    idx_t t = n;
    /* factor 2 */
    if (t % 2 == 0) {
        primes[(*cnt)++] = 2;
        while (t % 2 == 0) t /= 2;
    }
    /* factor odd primes */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            primes[(*cnt)++] = (int)p;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) primes[(*cnt)++] = (int)t;
}

/* Helper: test whether x is a prime power.
 * If x is a prime power, returns the base prime (>=2).
 * Otherwise returns 1. */
static seq_t prime_power_base(idx_t x)
{
    if (x == 1) return (seq_t)1;
    idx_t t = x;
    seq_t base = 0;
    /* test divisibility by 2 first */
    if (t % 2 == 0) {
        base = 2;
        while (t % 2 == 0) t /= 2;
    }
    /* test odd divisors */
    for (idx_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            if (base == 0) base = (seq_t)p;
            else if (base != (seq_t)p) return (seq_t)1;   /* more than one distinct prime */
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) {                /* remaining prime factor */
        if (base == 0) base = (seq_t)t;
        else if (base != (seq_t)t) return (seq_t)1;
    }
    return base;
}

/* Recursive enumeration of divisors built from distinct primes only.
 * Only divisors with all exponents 0 or 1 are considered, because μ(d)=0 otherwise.
 */
static void rec_sum(idx_t n, idx_t cur, int mu, const int *primes, int cnt, int pos, seq_t *sum)
{
    if (pos == cnt) {
        idx_t rem = n / cur;                 /* m = n/d */
        seq_t b = prime_power_base(rem);     /* b(m) */
        *sum += (seq_t)mu * b;
        return;
    }
    /* exclude current prime */
    rec_sum(n, cur, mu, primes, cnt, pos + 1, sum);
    /* include current prime */
    rec_sum(n, cur * (idx_t)primes[pos], -mu, primes, cnt, pos + 1, sum);
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;    /* offset starts at 1, behaviour below offset doesn't matter */

    int primes[64];
    int cnt;
    distinct_primes(n, primes, &cnt);

    seq_t result = 0;
    rec_sum(n, (idx_t)1, 1, primes, cnt, 0, &result);
    return result;
}
