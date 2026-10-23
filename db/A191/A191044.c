/*
 * A191044  Primes that are squares mod 71.
 * https://oeis.org/A191044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, powmod64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime that is a quadratic residue modulo 71. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/powmod64.h"

#define MOD 71ULL

/* Cache of qualifying primes. */
static seq_t  *prime_cache = NULL;   /* stored primes          */
static size_t cache_used  = 0;        /* number currently stored */
static size_t cache_cap   = 0;        /* allocated capacity      */
static idx_t  candidate   = 2;        /* next integer to test    */

/* Table of quadratic residues modulo MOD. */
static bool resid[MOD];
static bool resid_inited = false;

/* Initialise the residues table. */
static void init_resid(void)
{
    if (resid_inited) return;
    for (int r = 0; r < (int)MOD; ++r) {
        if (r == 0) {
            resid[r] = true;                 /* 0 is a square.        */
        } else {
            resid[r] = (powmod64((uint64_t)r,
                                  (MOD - 1) / 2,
                                  MOD) == 1);
        }
    }
    resid_inited = true;
}

/* Expand the cache so that it contains at least n qualifying primes. */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;
    init_resid();

    /* Allocate initial capacity if needed. */
    if (cache_cap == 0) {
        cache_cap     = 8;
        prime_cache   = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!prime_cache) abort();          /* Out of memory.         */
    }

    while (cache_used < (size_t)n) {
        /* Skip 71 itself (its residue is 0). */
        if (candidate == MOD) {
            candidate++;
            continue;
        }

        if (is_prime(candidate)) {
            uint64_t r = candidate % MOD;
            if (resid[(int)r]) {
                /* Grow the cache if necessary. */
                if (cache_used + 1 > cache_cap) {
                    size_t new_cap = cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(prime_cache,
                                                  new_cap * sizeof(seq_t));
                    if (!tmp) abort();          /* Out of memory.       */
                    prime_cache = tmp;
                    cache_cap   = new_cap;
                }
                prime_cache[cache_used] = (seq_t)candidate;
                cache_used++;
            }
        }
        candidate++;
    }
}

/* Public interface: a(n) = nth prime that is a quadratic residue modulo 71. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* Undefined for n=0, harmless. */
    ensure_cache(n);
    return prime_cache[(size_t)(n - 1)];
}
