/*
 * A038899  26 is a square mod p.
 * https://oeis.org/A038899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p for which 26 is a quadratic residue modulo p
   (including the primes 2 and 13).  a(1) = 2, a(2) = 5, a(3) = 11, … */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod); */
#include "../utils/is_prime.h"  /* bool is_prime(idx_t n); */

/* Grow a dynamic array of seq_t values. */
static void grow_seq(seq_t **p, size_t *cap, size_t need)
{
    if (*cap < need) {
        size_t newcap = (*cap == 0 ? 8 : *cap << 1);
        while (newcap < need) newcap <<= 1;
        *p = (seq_t *)realloc(*p, newcap * sizeof(seq_t));
        *cap = newcap;
    }
}

/* Cached sequence values and state. */
static seq_t  *seq_cache       = NULL;
static size_t  seq_len         = 0;
static size_t  seq_cap         = 0;
static uint64_t next_candidate = 2;  /* first number to test */

/* Test whether 26 is a quadratic residue modulo a prime p (p > 2). */
static bool residue26(uint64_t p)
{
    /* Legendre symbol (26|p) via Euler's criterion. */
    seq_t r = mod_pow((seq_t)26, (uint64_t)((p - 1) / 2), (seq_t)p);
    return r == (seq_t)1;
}

/* Ensure that at least n terms of the sequence have been generated. */
static void ensure_calculated(idx_t n)
{
    while (seq_len < (size_t)n) {
        /* Find next prime. */
        while (!is_prime(next_candidate)) {
            if (next_candidate == 2) next_candidate = 3;
            else next_candidate += 2;
        }

        /* Test the prime. */
        if (next_candidate == 2 ||
            next_candidate == 13 ||
            residue26(next_candidate)) {

            grow_seq(&seq_cache, &seq_cap, seq_len + 1);
            seq_cache[seq_len++] = (seq_t)next_candidate;
        }

        /* Advance to the next candidate. */
        if (next_candidate == 2) next_candidate = 3;
        else next_candidate += 2;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;       /* safety for out‑of‑range */
    ensure_calculated(n);
    return seq_cache[n - 1];
}
