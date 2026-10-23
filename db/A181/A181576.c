/*
 * A181576  Primes whose factorials end with a prime number of trailing 0's.
 * https://oeis.org/A181576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th prime p such that the number of trailing zeros
 *            in p! (i.e. the exponent of 5 in p!) is a prime number
 * Range:     u8 n <= 14, u16 n <= 801, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto, ensure_capacity, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime_small.h"
#include "../utils/ensure_capacity.h"

/* Cache and prime list structures */
static seq_t *term_cache    = NULL;   /* stores the sequence terms (primes) */
static idx_t  term_cnt     = 0;       /* number of terms already cached   */
static size_t term_cap     = 0;       /* allocated capacity of term_cache */

static uint64_t *prime_list = NULL;   /* list of primes up to LIMIT */
static size_t   prime_cnt     = 0;     /* number of primes in prime_list */
static idx_t    prime_idx     = 0;     /* next prime to process */

/* Limit of prime sieve: large enough for n up to about 10000. */
static const idx_t PRIME_LIMIT = 5000000;

/* Initialise primes (run once) */
static void init_primes(void) {
    if (prime_list != NULL) return;   /* already initialised */
    build_primes_upto(PRIME_LIMIT, &prime_list, &prime_cnt);
}

/* Compute trailing‑zero count of p! (number of 5's in p!) */
static uint64_t trailing_zeros(uint64_t p) {
    uint64_t z = 0;
    uint64_t pow5 = 5;
    while (pow5 <= p) {
        z += p / pow5;
        /* Guard against overflow when multiplying by 5 */
        if (pow5 > UINT64_MAX / 5) break;
        pow5 *= 5;
    }
    return z;
}

/* Generate sequence terms up to n (1‑based).  Returns the value a(n). */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;   /* out‑of‑range protection */

    /* Ensure prime list is ready */
    init_primes();

    /* Grow cache to at least n elements */
    while (term_cnt < n) {
        /* If we run out of primes in the sieve, we would need a larger
           sieve.  For n <= 10000 our limit is sufficient. */
        if (prime_idx >= prime_cnt) {
            /* In theory we would extend the sieve here, but per
               problem statement we only need up to about 10000 terms. */
            return (seq_t)0;
        }

        uint64_t p = prime_list[prime_idx++];
        uint64_t z = trailing_zeros(p);

        /* z is small enough to test primality with the small prime test. */
        if (is_prime_small((seq_t)z)) {
            /* Append p to the cache */
            ensure_capacity(term_cnt + 1, &term_cap, (void **)&term_cache, sizeof(seq_t));
            term_cache[term_cnt++] = (seq_t)p;
        }
    }

    /* Return the requested term (0‑based index = n-1). */
    return term_cache[n - 1];
}
