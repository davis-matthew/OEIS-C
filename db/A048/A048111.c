/*
 * A048111  Number of unitary divisors of n (A034444) < number of non-unitary
 *          divisors of n (A048105).
 * https://oeis.org/A048111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 7663, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Generates the nth integer n such that the number of its unitary
   divisors is smaller than the number of non‑unitary divisors,
   i.e. d(n) > 2^(ω(n)+1).
   (d = total divisor count, ω = number of distinct prime factors). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ---------- Prime sieve (up to a fixed limit) ---------- */
#define PRIME_LIMIT 1000000          /* > sqrt of expected max n */
static uint32_t primes[78498];      /* number of primes < 1,000,000 */
static size_t primes_cnt = 0;
static bool primes_ready = false;

static void build_primes(void)
{
    if (primes_ready) return;
    static bool marked[PRIME_LIMIT + 1] = {false};
    for (uint32_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (!marked[i]) {
            primes[primes_cnt++] = i;
            if ((uint64_t)i * i <= PRIME_LIMIT) {
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    marked[j] = true;
            }
        }
    }
    primes_ready = true;
}

/* ---------- Helper: test if an integer satisfies the condition ---------- */
static bool good_term(uint64_t n)
{
    uint64_t temp = n;
    size_t omega = 0;
    uint64_t d = 1;                     /* total divisor count */

    for (size_t idx = 0; idx < primes_cnt && (uint64_t)primes[idx] * primes[idx] <= temp; ++idx) {
        uint32_t p = primes[idx];
        if (temp % p == 0) {
            ++omega;
            int exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            d *= (uint64_t)(exp + 1);
        }
    }

    if (temp > 1) {                     /* remaining prime factor */
        ++omega;
        d *= 2;
    }

    /* Condition: d(n) > 2^(omega+1) */
    uint64_t bound = (uint64_t)1 << (omega + 1);
    return d > bound;
}

/* ---------- Cache for computed terms ---------- */
static seq_t *cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
static uint64_t last_checked = 1;     /* last integer examined */

/* add term to cache, expanding storage if necessary */
static void add_term(uint64_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(EXIT_FAILURE);
        cache = new_cache;
        cache_cap = new_cap;
    }
    cache[cache_len++] = (seq_t)val;
}

/* ---------- Main function requested by the problem statement ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for n=0 */
    build_primes();

    while (cache_len < n) {
        ++last_checked;
        if (good_term(last_checked))
            add_term(last_checked);
    }
    return cache[n - 1];
}
