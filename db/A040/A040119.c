/*
 * A040119  Primes p such that x^4 = 10 has a solution mod p.
 * https://oeis.org/A040119
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, int_sqrt, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A040119: primes p such that x^4 = 10 has a solution mod p.  */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/ensure_capacity.h"
#include "../utils/mod_pow.h"
#include "../utils/int_sqrt.h"

static seq_t  *seq        = NULL;
static size_t seq_len     = 0;
static size_t seq_cap     = 0;

static uint64_t *primes    = NULL;
static size_t   primes_len = 0;
static size_t   primes_cap = 0;

static uint64_t next_candidate = 3;   /* next odd number to test for primality */
static bool initialized = false;

/* Return true iff prime p satisfies x^4 = 10 (mod p). */
static bool good_prime(uint64_t p) {
    if (p == 2 || p == 5) return true;   /* 10 ≡ 0 (mod p) */
    uint64_t g;
    uint64_t pm1 = p - 1;
    if ((pm1 & 3) == 0)               /* p-1 divisible by 4 */
        g = 4;
    else if ((pm1 & 1) == 0)          /* p-1 divisible by 2 only */
        g = 2;
    else
        g = 1;
    uint64_t exp = pm1 / g;
    seq_t r = mod_pow((seq_t)10, exp, (seq_t)p);
    return r == 1;
}

/* Add p to the global prime list and, if it is a “good” prime, to the
   sequence of terms. */
static void add_prime(uint64_t p) {
    /* enlarge primes array if necessary */
    ensure_capacity(primes_len + 1, &primes_cap, (void**)&primes, sizeof(uint64_t));
    primes[primes_len++] = p;

    if (good_prime(p)) {
        ensure_capacity(seq_len + 1, &seq_cap, (void**)&seq, sizeof(seq_t));
        seq[seq_len++] = (seq_t)p;
    }
}

seq_t a(idx_t n) {
    if (!initialized) {
        /* initialise storage */
        ensure_capacity(1, &primes_cap, (void**)&primes, sizeof(uint64_t));
        primes_len = 0;
        add_prime(2);          /* first prime */
        next_candidate = 3;
        initialized = true;
    }

    while (seq_len < (size_t)n) {
        uint64_t cand = next_candidate;
        next_candidate += 2;   /* only odd candidates */

        /* trial division by known primes up to sqrt(cand) */
        bool isprime = true;
        uint64_t lim = int_sqrt(cand);
        for (size_t i = 0; i < primes_len && primes[i] <= lim; ++i) {
            if (cand % primes[i] == 0) {
                isprime = false;
                break;
            }
        }
        if (isprime) {
            add_prime(cand);
        }
    }
    return seq[n-1];
}
