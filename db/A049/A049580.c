/*
 * A049580  Primes p such that x^48 = 2 has a solution mod p.
 * https://oeis.org/A049580
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd_int, is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A049580:
   a(n) is the nth prime p for which the congruence x^48 ≡ 2 (mod p) has a
   solution (prime 2 is included).  The value is found by testing successive
   primes until the required number of solutions is reached.  The result
   depends only on the sequence of primes, so calls in any order are fast
   due to memoisation. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Utility functions provided by the build system */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/gcd_int.h"    /* int gcd_int(int a, int b); */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */

static seq_t *seq_cache = NULL;   /* array of computed terms */
static size_t seq_len  = 0;       /* number of terms currently stored */
static size_t seq_cap  = 0;       /* allocated capacity */

static uint64_t next_candidate = 2;  /* next integer to test for primality */

/* Ensure that seq_cache has at least 'needed' elements. */
static void ensure_seq_capacity(size_t needed)
{
    if (seq_cap < needed) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        if (new_cap < needed) new_cap = needed;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Append a new term to the cache. */
static void append_seq_value(seq_t val)
{
    ensure_seq_capacity(seq_len + 1);
    seq_cache[seq_len++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for offsets below 1 */

    /* Generate terms until we have at least n of them. */
    while (seq_len < (size_t)n) {
        /* Find the next prime candidate. */
        while (!is_prime((idx_t)next_candidate))
            ++next_candidate;

        uint64_t p = next_candidate;
        ++next_candidate;         /* advance for the next search */

        bool ok = false;

        if (p == 2) {
            /* 0^48 = 2 (mod 2) holds trivially. */
            ok = true;
        } else {
            uint64_t m = p - 1;            /* size of multiplicative group mod p */
            int r = m % 48;                /* r ∈ [0,47] */
            int g = gcd_int(48, r);        /* g = gcd(48, p-1) */
            uint64_t e = m / (uint64_t)g;  /* exponent to check */

            if (mod_pow((seq_t)2, e, (seq_t)p) == (seq_t)1)
                ok = true;
        }

        if (ok)
            append_seq_value((seq_t)p);
    }

    /* Return the requested n‑th term (1‑based). */
    return seq_cache[(size_t)n - 1];
}
