/*
 * A237453  Number of primes p < n with p*n + pi(p) prime, where pi(.) is
 *          given by A000720.
 * https://oeis.org/A237453
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ primes p < n : p*n + π(p) is prime }
 * Range:     u8 n <= 17, u16 n <= 387, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 96 terms through n = 9869 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

/* Build list of primes up to a limit */
#include "../utils/build_primes_upto.h"
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
/* Large‑integer primality test */

static idx_t max_prime_limit = 0;     /* maximum n for which primes are generated */
static uint64_t *prime_list   = NULL; /* array of primes up to max_prime_limit */
static size_t   prime_count    = 0;    /* number of primes in prime_list */

/* Extend the prime table up to “limit” (inclusive). */
static void extend_primes(idx_t limit)
{
    if (limit <= max_prime_limit) return;

    uint64_t *new_list = NULL;
    size_t   new_cnt   = 0;

    /* The helper allocates and returns a freshly allocated array of primes. */
    build_primes_upto(limit, &new_list, &new_cnt);

    /* Replace the old table. */
    free(prime_list);
    prime_list   = new_list;
    prime_count  = new_cnt;
    max_prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n <= 2) return 0;          /* no prime < n */

    /* Ensure we have all primes < n ready. */
    extend_primes(n - 1);

    seq_t count = 0;

    /* Loop over primes p < n.  The array is sorted, so we stop when p >= n. */
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p >= n) break;

        /* π(p) is the index of p in the prime list plus one. */
        idx_t pi_p = (idx_t)i + 1;

        /* Compute p*n + π(p) with 128‑bit arithmetic to avoid overflow. */
        __uint128_t val128 = (__uint128_t)p * (__uint128_t)n + (__uint128_t)pi_p;

        /* Convert to seq_t; the test harness guarantees val128 fits in seq_t. */
        seq_t val = (seq_t)val128;

        if (is_prime_big(val)) ++count;
    }

    return count;
}
