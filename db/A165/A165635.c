/*
 * A165635  Primes of the form (p^2 - 3)/2 where p is also prime.
 * https://oeis.org/A165635
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 56 terms through n = 960 (2026-11-03)
 * Helpers:   is_prime_u64, isqrt, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the n-th prime of the form (p^2 - 3)/2 where p itself is
   prime. It generates primes p incrementally, tests the formula for primality,
   stores the results, and memoises them for fast repeated calls. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/isqrt.h"
#include "../utils/mem.h"
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

seq_t a(idx_t n)
{
    /* Static memoisation tables */
    static idx_t max_n = 0;        /* number of terms already computed */
    static seq_t *vals   = NULL;     /* array of sequence values      */
    static size_t vals_cap = 0;      /* capacity of vals              */

    static uint64_t *primes   = NULL;/* list of primes found so far   */
    static size_t primes_cap  = 0;   /* capacity of primes array      */
    static size_t primes_cnt  = 0;   /* how many primes stored        */

    static uint64_t candidate = 3;   /* next odd number to test       */

    /* Initialise static structures on first call */
    if (vals == NULL) {
        vals_cap = 64;
        vals = mem_new(seq_t, vals_cap);

        primes_cap = 128;
        primes = (uint64_t*)malloc(primes_cap * sizeof(uint64_t));
        primes_cnt = 0;
        primes[primes_cnt++] = 2;        /* keep 2 for divisibility tests */
    }

    /* Extend the cache until we have the required n-terms */
    while (max_n < n) {
        uint64_t p = candidate;
        bool is_p_prime = true;

        /* Test primality of candidate p by trial division using primes list */
        idx_t sq = isqrt(p);
        for (size_t i = 0; i < primes_cnt && primes[i] <= sq; ++i) {
            if (p % primes[i] == 0) {
                is_p_prime = false;
                break;
            }
        }

        if (is_p_prime) {
            /* Store the new prime */
            if (primes_cnt == primes_cap) {
                primes_cap <<= 1;
                primes = (uint64_t*)realloc(primes, primes_cap * sizeof(uint64_t));
            }
            primes[primes_cnt++] = p;

            /* Compute q = (p^2 - 3)/2 using an intermediate 128‑bit type to avoid
               overflow of seq_t for small builds. */
            unsigned __int128 qp128 = (unsigned __int128)p * (unsigned __int128)p;
            unsigned __int128 q128  = (qp128 - 3) / 2;
            seq_t q_val = (seq_t)q128;

            /* Check that q fits into seq_t.  If it overflows, skip this term. */
            if ((unsigned __int128)q_val == q128 && is_prime_big(q_val)) {
                /* Ensure capacity of the results array */
                if (max_n >= vals_cap) {
                    vals_cap <<= 1;
                    vals = mem_resize(seq_t, vals, vals_cap);
                }
                vals[max_n++] = q_val;
            }
        }

        candidate += 2;   /* next odd integer */
    }

    return vals[n - 1];
}
