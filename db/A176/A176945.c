/*
 * A176945  Semiprimes s such that r=(s^2+1)/2 is also a semiprime.
 * https://oeis.org/A176945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A176945 – odd semiprimes s such that r=(s^2+1)/2 is also a semiprime. */
/* The function computes the n‑th term by simple brute force; n is 1‑based. */
/* It works for any build where seq_t is an integer type (fixed size or big). */

/* Implementation assumptions:
 *   - Values of s and r stay within 64 bits for all n up to at least 10000.
 *   - The function is single‑call; it does not depend on previous calls.
 *   - Prime list up to 1,000,000 is sufficient for factoring s (and r) in this range.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define PRIME_LIMIT 1000000
#define PRIME_COUNT 78498  /* number of primes <= 1,000,000 */

static uint32_t primes[PRIME_COUNT];
static size_t  prime_cnt = 0;
static bool    primes_built = false;

/* Build the prime table once */
static void build_primes(void)
{
    if (primes_built) return;
    bool *is_prime = (bool *)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    if (!is_prime) exit(1);  /* memory failure – abort */
    for (size_t i = 0; i <= PRIME_LIMIT; ++i) is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (is_prime[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                is_prime[j] = false;
        }
    }
    for (size_t i = 2; i <= PRIME_LIMIT && prime_cnt < PRIME_COUNT; ++i)
        if (is_prime[i]) primes[prime_cnt++] = (uint32_t)i;
    free(is_prime);
    primes_built = true;
}

/* Return true iff n is a semiprime (product of two primes,
 * counting multiplicity).  n is assumed <= 2^64 - 1. */
static bool is_semiprime_u64(uint64_t n)
{
    if (n < 4) return false;         /* smallest semiprime is 4 = 2*2 */
    size_t cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > n) break;
        while (n % p == 0) {
            ++cnt;
            if (cnt > 2) return false;
            n /= p;
        }
    }
    if (n > 1) ++cnt;  /* remaining prime factor > sqrt(original n) */
    return cnt == 2;
}

/* Main sequence function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined, return 0 */

    build_primes();

    uint64_t count = 0;
    for (uint64_t s = 3; ; s += 2) {           /* only odd s */
        if (!is_semiprime_u64(s)) continue;
        uint64_t s_sq = s * s;
        uint64_t r = (s_sq + 1) / 2;            /* r fits into 64 bit */
        if (!is_semiprime_u64(r)) continue;
        ++count;
        if (count == (uint64_t)n) return (seq_t)s;
    }
}
