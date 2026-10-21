/*
 * A092386  Let p(n) = n-th prime; a(n) = largest prime factor of (p(n+1)^2 -
 *          p(n)*p(n+2)).
 * https://oeis.org/A092386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = largest prime factor of (p(n+1)^2 - p(n)*p(n+2)) where
 *            p(k) is the k-th prime (p(0) = 2). The function works for all
 *            non‑negative n and returns 1 when the argument is ±1 or 0
 * Range:     u8 n <= 43, u16 n <= 1049, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PRIME_LIMIT 400000   /* enough for n up to about 20000 */
#define INITIAL_PRIME_CAP 512

static uint32_t *primes = NULL;
static size_t primes_cnt = 0;
static bool primes_built = false;

/* Build all primes <= PRIME_LIMIT using a simple sieve. */
static void build_primes(void)
{
    if (primes_built) return;
    primes_built = true;

    bool *sieve = (bool*)malloc((PRIME_LIMIT + 1) * sizeof(bool));
    memset(sieve, 1, PRIME_LIMIT + 1);
    sieve[0] = sieve[1] = 0;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = 0;
        }
    }

    primes_cnt = 0;
    size_t cap = INITIAL_PRIME_CAP;
    primes = (uint32_t*)malloc(cap * sizeof(uint32_t));
    for (size_t i = 2; i <= PRIME_LIMIT; ++i) {
        if (sieve[i]) {
            if (primes_cnt == cap) {
                cap *= 2;
                primes = (uint32_t*)realloc(primes, cap * sizeof(uint32_t));
            }
            primes[primes_cnt++] = (uint32_t)i;
        }
    }
    free(sieve);
}

/* Return the k-th prime (p(0)=2).  The caller must ensure k < primes_cnt. */
static uint32_t get_prime_at(idx_t k)
{
    if (!primes_built) build_primes();
    return primes[k];
}

/* Largest prime factor of a non‑negative 64‑bit integer x. */
static seq_t largest_prime_factor(uint64_t x)
{
    if (x <= 1) return (seq_t)1;
    seq_t largest = 1;
    for (size_t i = 0; i < primes_cnt; ++i) {
        uint32_t p = primes[i];
        uint64_t pp = (uint64_t)p * (uint64_t)p;
        if (pp > x) break;
        if (x % p == 0) {
            largest = p;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1 && x > largest) largest = x;
    return largest;
}

/* The sequence function required by the judge. */
seq_t a(idx_t n)
{
    /* Ensure we have enough primes. */
    if (!primes_built) build_primes();
    if (n + 2 >= primes_cnt) {
        /* The chosen PRIME_LIMIT is safe for n up to ~20000. */
        return (seq_t)0;  /* out of range; not expected in tests */
    }

    uint32_t p_n   = get_prime_at(n);
    uint32_t p_np1 = get_prime_at(n + 1);
    uint32_t p_np2 = get_prime_at(n + 2);

    uint64_t term = (uint64_t)p_np1 * (uint64_t)p_np1;
    uint64_t prod = (uint64_t)p_n * (uint64_t)p_np2;
    int64_t diff = (int64_t)term - (int64_t)prod;
    uint64_t val = diff >= 0 ? (uint64_t)diff : (uint64_t)(-diff);

    return largest_prime_factor(val);
}
