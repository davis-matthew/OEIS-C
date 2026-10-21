/*
 * A075177  Indices of additive primes, see A046704.
 * https://oeis.org/A075177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = PrimePi(A046704(n)) (indices of additive primes)
 * Range:     u8 n <= 136, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* --------------- Static state ---------------------------------------- */
static seq_t  *cache = NULL;          /* cache[n-1] = a(n) */
static size_t cache_len = 0, cache_cap = 0;

static uint32_t *primes = NULL;       /* list of primes found so far */
static size_t   primes_len = 0, primes_cap = 0;

static uint32_t next = 2;             /* next integer to test for primality */
static uint32_t pcount = 0;           /* number of primes seen so far */

/* --------------- Utility functions ---------------------------------- */
static inline bool is_prime_uint32(uint32_t x)
{
    if (x < 2) return false;
    if (x % 2 == 0) return x == 2;
    for (uint32_t i = 3; (uint64_t)i * i <= x; i += 2)
        if (x % i == 0) return false;
    return true;
}

static inline uint32_t digit_sum(uint32_t n)
{
    uint32_t s = 0;
    while (n) { s += n % 10; n /= 10; }
    return s;
}

/* --------------- Main sequence function ------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* undefined, but safe for idx<1 */

    /* Grow cache until it contains n elements */
    while (cache_len < (size_t)n) {
        /* Test primality of 'next' */
        bool prime;
        if (next <= 0xFFFFFFFF) prime = is_prime_uint32(next);
        else                    prime = is_prime_uint32((uint32_t)next); /* fallback */

        if (prime) {
            /* store the prime */
            if (primes_len == primes_cap) {
                primes_cap = (primes_cap == 0) ? 16 : primes_cap * 2;
                primes = (uint32_t*)realloc(primes, primes_cap * sizeof(uint32_t));
            }
            primes[primes_len++] = next;
            ++pcount;

            /* digit–sum primality test */
            uint32_t sum = digit_sum(next);
            if (is_prime_uint32(sum)) {
                /* new additive prime: record its index among all primes */
                if (cache_len == cache_cap) {
                    cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)pcount;
            }
        }
        ++next;
    }
    return cache[n - 1];
}
