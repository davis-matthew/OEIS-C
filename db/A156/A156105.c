/*
 * A156105  Primes p such that p + 72 is also prime.
 * https://oeis.org/A156105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n‑th prime p such that p‑72 is also prime
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 870 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation searches primes on demand and caches the found
   terms. It uses the supplied is_prime_big function for primality testing. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
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

/* Cache for sequence terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Next integer to test for being a qualifying prime */
static uint64_t next_candidate = 2;

/* Ensure the cache contains at least 'need' terms */
static void ensure_terms(idx_t need)
{
    /* Expand the cache in powers of two */
    if (cache_cap < (size_t)need) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < (size_t)need) new_cap = need;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();      /* allocation failed */
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Search for further qualifying primes until we have 'need' terms */
    while (cache_len < (size_t)need) {
        seq_t cand = (seq_t)next_candidate;         /* current candidate   */
        seq_t cand_plus = cand + 72;                /* p + 72              */

        /* Is cand a prime and is cand_plus a prime? */
        if (is_prime_big(cand) && is_prime_big(cand_plus)) {
            cache[cache_len++] = cand;              /* store it           */
        }

        ++next_candidate;  /* advance for the next trial              */
    }
}

/* Return the n-th term of the sequence (a(1) = 7) */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for non‑positive indices */
    ensure_terms(n);
    return cache[n - 1];
}
