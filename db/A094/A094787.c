/*
 * A094787  a(n) = smallest prime p such that p + n is a perfect power m^k, k
 *          >= 2.
 * https://oeis.org/A094787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p + n is a perfect power
 *            (exponent k ≥ 2)
 * Range:     u8 n <= 27, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 81 (2026-11-01)
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime.h"

/* Compare mid^k to limit: returns -1 if <, 0 if ==, 1 if > */
static int compare_pow_seq(const seq_t mid, uint64_t k, const seq_t limit)
{
    seq_t res = 1;
    for (uint64_t i = 0; i < k; ++i)
    {
        res *= mid;
        if (res > limit) return 1;
    }
    if (res == limit) return 0;
    return -1;
}

/* Compute floor of k-th root of x (the largest m such that m^k ≤ x) */
static seq_t kth_root_floor(const seq_t x, uint64_t k)
{
    seq_t lo = 1;
    seq_t hi = 2;

    /* Find an upper bound */
    while (compare_pow_seq(hi, k, x) <= 0)
    {
        hi = hi + hi;          /* double hi */
    }

    /* Binary search between lo and hi */
    while (lo < hi)
    {
        seq_t mid = lo + (hi - lo) / 2;
        int cmp = compare_pow_seq(mid, k, x);
        if (cmp == 0) return mid;
        if (cmp < 0)
            lo = mid + 1;
        else
            hi = mid;
    }

    /* lo == hi: check if lo^k == x, otherwise lo-1 is the floor root */
    if (compare_pow_seq(lo, k, x) == 0)
        return lo;
    else
        return lo - 1;
}

/* Test whether x > 1 and is a perfect power with exponent k ≥ 2 */
static bool is_perfect_power(const seq_t x)
{
    if (x < 4) return false;   /* 1,2,3 are not perfect powers */

    seq_t two = 2;
    for (uint64_t k = 2; ; ++k)
    {
        /* If 2^k > x, further exponents will be > x as well */
        if (compare_pow_seq(two, k, x) > 0)
            break;

        seq_t m = kth_root_floor(x, k);
        seq_t val = 1;
        for (uint64_t i = 0; i < k; ++i)
            val *= m;

        if (val == x)
            return true;
    }
    return false;
}

/* Maximum prime to sieve initially. Adjust if larger p are needed. */
#define PRIME_LIMIT 5000000

/* Global storage for primes */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* Initialise prime table once */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Allocate sieve */
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++prime_cnt;

    /* Allocate array and store primes */
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
    primes_initialized = true;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 */
    init_primes();

    /* Search primes within the sieve first */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t x = (seq_t)n + p;
        if (is_perfect_power(x))
            return p;
    }

    /* If not found, continue with larger primes */
    idx_t cand = PRIME_LIMIT + 1;
    if ((cand & 1) == 0) cand++;   /* make odd */

    while (1)
    {
        if (is_prime(cand))
        {
            seq_t x = (seq_t)n + (seq_t)cand;
            if (is_perfect_power(x))
                return (seq_t)cand;
        }
        cand += 2;   /* next odd number */
    }
}
