/*
 * A038919  Primes p such that 41 is a square mod p.
 * https://oeis.org/A038919
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A038919: primes p such that 41 is a quadratic residue modulo p
   (searching primes and testing 41^((p‑1)/2) ≡ 1 (mod p)). */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x)          */
#include "../utils/mod_pow.h"    /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod) */

/* static cache of computed terms */
static seq_t *cache   = NULL;   /* pointer to the cache array */
static size_t cache_cnt = 0;    /* number of terms stored */
static size_t cache_cap = 0;    /* current allocated capacity */

/* enlarge the cache if necessary */
static void ensure_capacity(size_t need)
{
    if (need <= cache_cap)
        return;
    size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, the program will abort, as required by the build. */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the n‑th term (1‑based) of A038919. */
seq_t a(idx_t n)
{
    /* If the desired term is already cached, return it. */
    if (n <= cache_cnt)
        return cache[n - 1];

    /* First term MUST be 2.  Initialise the cache if empty. */
    if (cache_cnt == 0) {
        ensure_capacity(1);
        cache[0] = (seq_t)2;
        cache_cnt = 1;
    }

    /* Where to start searching for further primes. */
    uint64_t last_prime = (uint64_t)cache[cache_cnt - 1];
    uint64_t candidate   = last_prime + 1;
    if (candidate <= 2)           /* insure we start from 3 */
        candidate = 3;
    if (candidate % 2 == 0)       /* make it odd after 2 */
        candidate++;

    /* Fill the cache until it contains at least n terms. */
    while (cache_cnt < n) {
        /* Find the next prime candidate. */
        while (!is_prime(candidate))
            candidate += 2;       /* skip even numbers */

        /* Is 41 a quadratic residue modulo this prime? */
        bool ok = false;
        if (candidate == 41) {
            ok = true;            /* 0^2 ≡ 0 (mod 41) */
        } else {
            /* Use Euler’s criterion: 41^((p‑1)/2) ≡ 1 (mod p) */
            seq_t test = mod_pow((seq_t)41,
                                 (candidate - 1) / 2,
                                 (seq_t)candidate);
            ok = test == (seq_t)1;
        }

        if (ok) {
            /* Store the new term. */
            ensure_capacity(cache_cnt + 1);
            cache[cache_cnt++] = (seq_t)candidate;
        }

        /* Advance to the next odd number after the prime just found. */
        candidate += 2;
    }

    return cache[n - 1];
}
