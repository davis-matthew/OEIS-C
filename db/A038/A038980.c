/*
 * A038980  Primes p such that 91 is not a square mod p.
 * https://oeis.org/A038980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes p such that 91 is not a quadratic residue modulo p
 * Range:     u8 n <= 29, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/mod_pow.h"

static seq_t   *cache = NULL;   /* stored terms of the sequence         */
static size_t  cached_len  = 0;  /* number of terms stored               */
static size_t  cached_cap  = 0;  /* capacity of the cache (for realloc) */
static idx_t   last_checked = 1; /* last integer inspected              */

/* Helper:  Is 91 a quadratic non‑residue modulo p?  p must be an odd prime
   greater than 2 and not dividing 91.  Returns true iff the Legendre
   symbol (91|p) = -1. */
static bool is_non_residue(uint64_t p)
{
    /* Euler's criterion: a^{(p-1)/2} mod p == 1  -> residue
       == p-1 (i.e. -1 mod p) -> nonresidue.          */
    uint64_t exp = (p - 1) / 2;
    seq_t   base = (seq_t)91;
    seq_t   mod  = (seq_t)p;
    seq_t   r    = mod_pow(base, exp, mod);
    seq_t   one  = 1;
    if (r == one) return false;                    /* residue */
    seq_t   minus_one = mod - one;                 /* -1 mod p  */
    return r == minus_one;                         /* nonresidue? */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0 */

    /* Ensure we have at least n terms cached. */
    while (cached_len < (size_t)n) {
        uint64_t cand = last_checked + 1;
        last_checked = cand;

        /* Skip composites. */
        while (!is_prime((idx_t)cand)) {
            cand++;
            last_checked = cand;
        }

        /* cand is prime.  Decide if it belongs to the sequence. */
        if (cand == 2) {
            /* 2 is included explicitly. */
        } else if (cand == 7 || cand == 13) {
            /* 91 ≡ 0 mod 7 or 13 → 91 is a square => skip. */
            cand = 0;
        } else if (is_non_residue(cand)) {
            /* True non‑residue, add to cache. */
        } else {
            /* Not a non‑residue, skip. */
            cand = 0;
        }

        if (cand != 0) {
            if (cached_len >= cached_cap) {
                size_t new_cap = cached_cap ? cached_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cached_cap = new_cap;
            }
            cache[cached_len++] = (seq_t)cand;
        }
    }

    return cache[n - 1];
}
