/*
 * A077004  Duplicate of A002034.
 * https://oeis.org/A077004
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = least m such that n divides m!
 * Range:     i8 n <= 29, i16 n <= 29, i32 n <= 29, i64 n <= 29, i128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (duplicate of OEIS A002034, offset 1) */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* sieve of primes up to 1 000 000 (sufficient for n ≤ 10 000) */
static idx_t primes[78500];
static size_t prime_count = 0;

static void build_primes(void)
{
    if (prime_count) return;

    const idx_t LIMIT = 1000000;
    bool *sieve = (bool*)calloc(LIMIT + 1, sizeof(bool));
    if (!sieve) return;

    for (idx_t i = 2; i * i <= LIMIT; ++i) {
        if (!sieve[i]) {
            for (idx_t j = i * i; j <= LIMIT; j += i)
                sieve[j] = true;
        }
    }

    for (idx_t i = 2; i <= LIMIT; ++i)
        if (!sieve[i]) primes[prime_count++] = i;

    free(sieve);
}

/* minimal m such that exponent of prime p in m! is at least exp */
static idx_t min_m_for_prime_power(idx_t p, int exp)
{
    idx_t low = 1;
    idx_t high = p * (idx_t)exp;
    if (high < 2) high = 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t t = mid;
        idx_t cnt = 0;
        while (t) {
            t /= p;
            cnt += t;
        }
        if (cnt >= (idx_t)exp)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)1;   /* a(1) = 1 */

    build_primes();

    idx_t rem   = n;
    idx_t answer = 1;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = primes[i];
        if (p * p > rem) break;
        if (rem % p == 0) {
            int exp = 0;
            while (rem % p == 0) {
                rem /= p;
                ++exp;
            }
            idx_t m = min_m_for_prime_power(p, exp);
            if (m > answer) answer = m;
        }
    }

    if (rem > 1) {           /* remaining prime factor */
        idx_t p = rem;
        if (p > answer) answer = p;
    }

    return (seq_t)answer;
}
