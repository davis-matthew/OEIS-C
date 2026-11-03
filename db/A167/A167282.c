/*
 * A167282  Primes containing 777 as a substring.
 * https://oeis.org/A167282
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 50 terms through n = 886 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the sequence of primes whose decimal representation contains
   the substring "777".  a(n) is the nth such prime (a(1)=1777).  The
   implementation generates successive primes using the utility
   next_prime(uint64_t) and keeps only those that contain 777.  A
   static cache of found primes is maintained so that calls in any
   order are answered in O(1) after the required primes have been
   generated. */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/next_prime.h"
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

/* Return true iff n (in decimal) contains the substring 777. */
static bool contains_777(seq_t n)
{
    if (n < 777) return false;          /* cannot contain 777 */

    /* Extract digits in reverse order (least‑significant first). */
    unsigned char digits[200];           /* enough for typical values */
    size_t sz = 0;
    seq_t tmp = n;
    while (tmp > 0) {
        digits[sz++] = (unsigned char)(tmp % 10);
        tmp /= 10;
        if (sz >= sizeof(digits)) break; /* safety guard */
    }
    if (sz < 3) return false;

    /* Scan for three consecutive 7's in the original order. */
    for (size_t i = 2; i < sz; ++i) {
        if (digits[i]   == 7 &&
            digits[i-1] == 7 &&
            digits[i-2] == 7)
            return true;
    }
    return false;
}

/* Static cache for the sequence. */
static seq_t *prime_cache      = NULL;   /* array of primes */
static size_t cache_len        = 0;      /* number of cached primes */
static size_t cache_cap        = 0;      /* allocated capacity */
static uint64_t next_start     = 2;      /* next value to search for a prime */

/* Return the nth term of the sequence, 1‑based. */
seq_t a(idx_t n)
{
    /* Generate primes until we have at least n terms cached. */
    while (cache_len < (size_t)n)
    {
        /* Get the next prime after next_start. */
        uint64_t cand = next_prime((uint64_t)next_start);
        if (cand == 0) break;            /* should not happen */

        /* Advance the starting point for the next search. */
        next_start = cand + 1;

        /* Convert candidate to seq_t for the substring test. */
        seq_t pcand = (seq_t)cand;

        if (contains_777(pcand))
        {
            /* Grow cache if needed. */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0 ? 16 : cache_cap * 2);
                prime_cache = mem_resize(seq_t, prime_cache, cache_cap);
                if (!prime_cache) exit(EXIT_FAILURE); /* allocation failure */
            }
            prime_cache[cache_len++] = pcand;
        }
    }
    return prime_cache[(size_t)n - 1];
}
