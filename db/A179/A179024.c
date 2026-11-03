/*
 * A179024  Primes whose only prime digits are 2's.
 * https://oeis.org/A179024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 413, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9933 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime whose decimal digits contain only the prime
   digit 2 (and at least one 2), as in OEIS A179024.
   The implementation searches for primes incrementally and memoises
   the results, so the function works for arbitrary order of calls. */

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

/* Cache of discovered primes */
static seq_t *prime_cache   = NULL;  /* array of found primes          */
static size_t  cache_cap    = 0;     /* allocated array capacity       */
static size_t  cache_len    = 0;     /* number of primes stored        */
static idx_t   last_checked = 0;     /* largest integer examined so far */

/* ------------------------------------------------------------------ */
/* Helper: check that a decimal integer consists only of the prime
   digit 2 (digits 3,5,7 are forbidden) and contains at least one 2. */
static bool allowed_digits(idx_t x)
{
    bool has_two = false;
    while (x > 0) {
        unsigned int d = (unsigned int)(x % 10);
        if (d == 3 || d == 5 || d == 7) return false;
        if (d == 2) has_two = true;
        x /= 10;
    }
    return has_two;
}

/* ------------------------------------------------------------------ */
/* Append a new prime to the cache, expanding the storage if needed. */
static void append_prime(seq_t p)
{
    if (cache_len == cache_cap) {
        cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
        if (!prime_cache) abort();
    }
    prime_cache[cache_len++] = p;
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n primes. */
    while (cache_len < n) {
        /* Search for the next candidate integer. */
        for (idx_t cand = last_checked + 1; ; ++cand) {
            if (!allowed_digits(cand)) continue;

            /* Convert to seq_t for prime test. */
            seq_t cand_seq = (seq_t)cand;
            if (is_prime_big(cand_seq)) {
                append_prime(cand_seq);
                last_checked = cand;
                break;   /* prime found; advance to next missing slot */
            }
        }
    }
    return prime_cache[n - 1];
}
