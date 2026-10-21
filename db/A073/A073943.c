/*
 * A073943  a(n) = smallest m such that n-th prime divides m^m + 1.
 * https://oeis.org/A073943
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = smallest m such that the n-th prime divides m^m + 1
 * Range:     u8 n <= 31, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/isPrime.h"
#include "../utils/mod_pow.h"

static seq_t primes[200000];    /* store primes; ample for n up to ~200k */
static size_t prime_count = 0;  /* number of primes stored */
static idx_t last_candidate = 1;/* last number tested for primality */

/* Ensure that at least 'need' primes are generated and stored. */
static void generate_primes_up_to(size_t need)
{
    if (prime_count >= need) return;

    idx_t cand = last_candidate + 1;
    while (prime_count < need) {
        if (isPrime(cand)) {
            primes[prime_count++] = (seq_t)cand;
        }
        cand++;
    }
    last_candidate = cand - 1;
}

/* Return the n-th prime (1‑based, n >= 1). */
static seq_t nth_prime(idx_t n)
{
    generate_primes_up_to((size_t)n);
    return primes[(size_t)(n - 1)];
}

/* Main function required by the specification. */
seq_t a(idx_t n)
{
    /* n is 1‑based */
    seq_t p = nth_prime(n);

    /* Special case: the first prime is 2, a(1) = 1. */
    if (p == (seq_t)2) {
        return (seq_t)1;
    }

    idx_t m = 1;
    while (true) {
        seq_t base = (seq_t)m % p; /* base modulo p */
        if (base == 0) {
            ++m;
            continue; /* skips multiples of p */
        }
        seq_t val = mod_pow(base, m, p);   /* base^m mod p */
        if (((val + (seq_t)1) % p) == 0) { /* check if p divides m^m + 1 */
            return (seq_t)m;
        }
        ++m;
    }
}
