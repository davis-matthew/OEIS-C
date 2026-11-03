/*
 * A039639  Fixed point of "k -> k/2 or (k-1)/2 until result is prime",
 *          starting with prime(n)+1.
 * https://oeis.org/A039639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 56, u16 n <= 7539, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9727 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Fixed point of the map k -> k/2 or (k-1)/2 until the result is prime,
   starting with prime(n)+1.  For each n ≥ 1, a(n) is the first prime
   reached by that iteration. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions (provided by the OEIS build system).  Their
   corresponding headers are located in ../utils/.  Include only what
   we need here. */
#include <stddef.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache of the first N primes.  It is extended lazily on demand. */
static idx_t *prime_list = NULL;      /* pointer to array of primes */
static size_t prime_count = 0;        /* number of primes currently stored */
static idx_t  prime_limit = 0;        /* current upper bound used for the sieve */

/* Ensure that the prime cache contains at least the first 'n' primes. */
static void
ensure_primes(idx_t n)
{
    /* If we already have enough primes, nothing to do. */
    if (prime_count >= (size_t)n)
        return;

    /* Determine a new sieve limit.  Start with a modest bound and
       double it until we have at least n primes. */
    if (prime_limit == 0) {
        /* For small n use a small fixed limit; otherwise start with n*15
           which is a safe over‑estimate for the n‑th prime up to
           millions. */
        prime_limit = (n < 6) ? 15U : n * 15U;
    } else {
        /* Grow exponentially to keep the number of re‑sieves small. */
        prime_limit *= 2;
    }

    /* Rebuild the sieve with the new limit. */
    idx_t *tmp;            /* temporary pointer for the new list */
    size_t tmp_cnt;        /* number of primes found */

    build_primes(prime_limit, &tmp, &tmp_cnt);

    /* Replace the old list (if any). */
    free(prime_list);               /* free old list (may be NULL) */
    prime_list = tmp;
    prime_count = tmp_cnt;

    /* If we still do not have enough primes, repeat the process. */
    if (prime_count < (size_t)n)
        ensure_primes(n);
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for n = 0 – guard against UB */

    /* Make sure we have at least n primes in the cache. */
    ensure_primes(n);

    /* Start from prime(n)+1.  prime_list is zero‑based. */
    seq_t k = (seq_t)prime_list[ (size_t)n - 1 ] + (seq_t)1;

    /* Iterate the map until a prime is reached. */
    while (!is_prime_big(k)) {
        if ((k & (seq_t)1) == 0)      /* k even  -> k/2 */
            k >>= 1;
        else                          /* k odd   -> (k-1)/2 */
            k = (k - (seq_t)1) >> 1;
    }

    return k;
}
