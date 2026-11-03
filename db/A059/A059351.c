/*
 * A059351  Primes p such that x^39 = 2 has no solution mod p.
 * https://oeis.org/A059351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 70 terms through n = 973 (2026-11-03)
 * Helpers:   gcd, gcd_u64, mem, next_prime, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059351
 *  A(n) = the n-th prime p such that the congruence
 *         x^39 ≡ 2 (mod p) has no solution.
 *  The condition for a prime p>2 is:
 *      let g = gcd(39, p-1)
 *      let e = (p-1)/g
 *      then the congruence has a solution iff 2^e ≡ 1 (mod p).
 *  So a prime p is in the sequence iff 2^e mod p ≠ 1.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include <string.h>
#include "../utils/gcd.h"
#include "../utils/gcd_u64.h"
#include "../utils/next_prime.h"
#include "../utils/powmod.h"
#include "../utils/mem.h"

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

static seq_t *primes_cache      = NULL;   /* memoised primes satisfying the condition */
static size_t cache_len          = 0;      /* number of cached primes */
static size_t cache_cap          = 0;      /* allocated capacity of primes_cache */
static uint64_t last_checked_prime = 2;    /* last prime processed (initially 2) */

/* Extend the cache until it contains at least n primes that satisfy the condition. */
static void _ensure_cache(idx_t n)
{
    while ((idx_t)cache_len < n) {
        /* get next prime > last_checked_prime */
        last_checked_prime = next_prime(last_checked_prime + 1);
        if (last_checked_prime == 0) /* overflow protection; should not happen for n≤10000 */
            break;

        /* Compute g = gcd(39, p-1) */
        uint64_t g = gcd_u64(39ULL, last_checked_prime - 1);
        uint64_t e = (last_checked_prime - 1) / g;

        /* test whether 2^e ≡ 1 (mod p) */
        seq_t res = powmod((seq_t)2, e, (seq_t)last_checked_prime);
        if (res != (seq_t)1) {
            /* prime satisfies the condition; store it */
            append_candidate(&primes_cache, &cache_len, &cache_cap, (seq_t)last_checked_prime);
        }
    }
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < (idx_t)1) return (seq_t)0;      /* undefined for n<1, but return 0 */
    _ensure_cache(n);
    return primes_cache[(size_t)(n - 1)];
}
