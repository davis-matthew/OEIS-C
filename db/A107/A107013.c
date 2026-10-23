/*
 * A107013  Primes of the form x^2-xy+25y^2, with x and y nonnegative.
 * https://oeis.org/A107013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   cmp_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of the form x^2 - x*y + 25*y^2 with x,y ≥ 0,
   returned in ascending order.  The implementation
   pre‑computes all such primes up to a fixed bound
   (50 000 000) and stores them in a static array.
   The function a(idx_t n) then returns the n‑th prime,
   or 0 if n exceeds the number of precomputed values. */

/* The following headers are safe for both C11 and C++17. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "../utils/cmp_uint64.h"

/* Configurable bound – large enough to contain the first
   several thousand terms of the sequence. */
static const uint64_t BOUND = 50000000ULL;   /* 50 million */

/* Static data used by the algorithm */
static uint8_t *sieve      = NULL;          /* bitset of primes up to BOUND   */
static uint64_t *prime_list = NULL;          /* sorted list of desired primes */
static idx_t     prime_count = 0;            /* number of primes stored       */
static bool      initialized = false;        /* have we built the tables yet?  */

/* --------------------------------------------------------------------- */

/* --------------------------------------------------------------------- */
/* Build a sieve of Eratosthenes up to BOUND.  The bitset is stored
   as an array of bytes; each byte holds eight flags.                 */
static void build_sieve(void)
{
    size_t sieve_bytes = (BOUND >> 3) + 1;
    sieve = (uint8_t *)calloc(sieve_bytes, 1);   /* all bits initially 0 */
    if (!sieve) return;                         /* allocation failed */

    /* Set all bits to 1 (except 0 and 1 later). */
    memset(sieve, 0xFF, sieve_bytes);

    /* 0 and 1 are not primes. */
    sieve[0] &= ~(1 << 0);
    sieve[0] &= ~(1 << 1);

    /* classic Eratosthenes */
    uint64_t limit = (uint64_t)sqrt((double)BOUND) + 1;
    for (uint64_t p = 2; p <= limit; ++p) {
        if (sieve[p >> 3] & (1U << (p & 7))) {  /* p is still marked prime */
            for (uint64_t m = p * p; m <= BOUND; m += p) {
                sieve[m >> 3] &= ~(1U << (m & 7));
            }
        }
    }
}

/* --------------------------------------------------------------------- */
/* Quick primality test using the pre‑built sieve.  For values > BOUND
   the function will always return false – this is OK because we only
   query inside the bound.                                              */
static inline bool is_prime_sieve(uint64_t v)
{
    if (v > BOUND) return false;
    return (sieve[v >> 3] & (1U << (v & 7))) != 0;
}

/* --------------------------------------------------------------------- */
/* Generate all primes of the prescribed quadratic form up to BOUND
   and store them in a sorted, deduplicated array.                       */
static void compute_primes(void)
{
    /* Ensure sieve is built first. */
    if (!sieve) build_sieve();

    /* Rough estimate of needed capacity. */
    size_t cap = 20000;
    prime_list = (uint64_t *)calloc(cap, sizeof(uint64_t));
    if (!prime_list) return;

    /* y must be at least 1; y_max chosen so that 25*y^2 <= BOUND. */
    uint64_t y_max = (uint64_t)sqrt((double)BOUND / 25.0) + 2;
    for (uint64_t y = 1; y <= y_max; ++y) {
        uint64_t y_term = 25ULL * y * y;          /* 25*y^2 */
        if (y_term > BOUND) break;

        for (uint64_t x = 0;; ++x) {
            /* Compute x^2 - x*y + 25*y^2 */
            uint64_t val = x * x - x * y + y_term;
            if (val > BOUND) break;
            if (val >= 2 && is_prime_sieve(val)) {
                /* Grow array if necessary */
                if (prime_count == cap) {
                    cap <<= 1; /* double the capacity */
                    uint64_t *tmp = (uint64_t *)realloc(prime_list, cap * sizeof(uint64_t));
                    if (!tmp) break;               /* memory failure: abort */
                    prime_list = tmp;
                }
                prime_list[prime_count++] = val;
            }
        }
    }

    /* Sort list and deduplicate */
    qsort(prime_list, prime_count, sizeof(uint64_t), cmp_uint64);
    idx_t uniq = 0;
    for (idx_t i = 0; i < prime_count; ++i) {
        if (i == 0 || prime_list[i] != prime_list[i - 1]) {
            prime_list[uniq++] = prime_list[i];
        }
    }
    prime_count = uniq;
}

/* --------------------------------------------------------------------- */
/* Main sequence function required by the framework.  It lazily
   generates the tables on first use.                                    */
seq_t a(idx_t n)
{
    if (!initialized) {
        compute_primes();
        initialized = true;
    }

    if (n == 0 || n > prime_count) {
        return (seq_t)0;        /* undefined or out of range */
    }
    return (seq_t)prime_list[n - 1];
}
