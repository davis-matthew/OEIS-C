/*
 * A235637  Primes whose base-7 representation also is the base-6
 *          representation of a prime.
 * https://oeis.org/A235637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 490, u32 n <= 14278, u64 n <= 14278,
 *            u128 n <= 14278, big any n
 * Validated: standard, pass, 69 terms through n = 9255 (2026-11-03)
 * Helpers:   build_primes, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find primes whose base‑7 representation uses only digits 0–5,
   and when that same digit sequence is interpreted as a base‑6
   number it is also prime. */

/* The implementation is a straightforward enumerator:
   1.  Generate primes incrementally using build_primes.
   2.  For each prime, convert its decimal representation to base‑7.
       Reject it if any digit ≥ 6 (since base‑6 digits must be < 6).
   3.  Interpret the accepted digit sequence in base‑6 to obtain q.
   4.  Test q for primality with is_prime_big.
   5.  Store qualifying primes in a static cache so that arbitrary
       order of calls is supported. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/build_primes.h"
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

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* static cache of found terms */
static seq_t *cache = NULL;   /* cached sequence values */
static size_t cache_len = 0;  /* how many values are cached */
static size_t cache_cap = 0;  /* capacity of the cache */

/* Test whether a prime satisfies the base-7 → base-6 property. */
static bool base7_to_base6_prime(seq_t p)
{
    seq_t q = 0;     /* value when digits are interpreted in base 6 */
    seq_t mul = 1;   /* current power of 6 */

    while (p > 0) {
        seq_t d = p % 7;
        /* Reject if digit would be invalid in base‑6. */
        if (d >= 6)
            return false;
        q += d * mul;
        mul *= 6;    /* safe for the expected range of primes  */
        p /= 7;
    }

    return is_prime_big(q);
}

/* Extend the cache until it contains at least n_needed terms. */
static void generate_upto(size_t n_needed)
{
    /* static state for incremental sieve */
    static idx_t sieve_limit = 0;      /* highest number sieved so far */
    static idx_t prime_start = 0;     /* index of first prime not yet processed */

    while (cache_len < n_needed) {
        /* Initialize or double the sieve limit. */
        if (sieve_limit == 0)
            sieve_limit = 1000000;
        else
            sieve_limit *= 2;

        /* Build all primes up to the new limit. */
        idx_t *prime_list = NULL;
        size_t prime_cnt = 0;
        build_primes(sieve_limit, &prime_list, &prime_cnt);

        /* Examine only the freshly added primes. */
        for (size_t i = prime_start; i < prime_cnt; ++i) {
            seq_t p = (seq_t)prime_list[i];
            if (base7_to_base6_prime(p))
                append_candidate(&cache, &cache_len, &cache_cap, p);
        }

        /* Clean up. */
        free(prime_list);
        prime_start = prime_cnt;
    }
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;             /* undefined for n < 1, but harmless */

    if (cache_len < (size_t)n)
        generate_upto((size_t)n);

    return cache[n - 1];       /* a(1) corresponds to index 0 */
}
