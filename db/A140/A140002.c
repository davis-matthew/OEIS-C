/*
 * A140002  Primes of the form 8*x^2 + 165*y^2.
 * https://oeis.org/A140002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 207, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function generates the n-th prime of the form 8*x^2 + 165*y^2.
   It does so by iterating through all primes that satisfy the
   congruence conditions known from the OEIS entry.  The residues
   that a prime of this form can take modulo 1320 are
   173, 197, 293, 413, 437, 557, 677, 893, 1157, 1253.  We test
   each such candidate for primality and keep the ones we find.
   This approach requires only the is_prime_big function from the
   provided utilities and a simple dynamic array for caching the
   generated primes.  The cache is built lazily and increments
   independently of the order of calls.  Complexity is
   essentially linear in the number of candidates examined.  The
   routine returns the exact value, whether it fits in 64 bits or
   requires arbitrary precision. */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

static const uint64_t residues[10] = {173,197,293,413,437,557,677,893,1157,1253};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static size_t cache_len = 0;
static uint64_t current_base = 0;
static int residue_idx = 0;

seq_t a(idx_t n)
{
    /* Ensure we have at least n primes in the cache. */
    while (cache_len < (size_t)n)
    {
        uint64_t candidate = current_base + residues[residue_idx];
        if (candidate >= 2 && is_prime_big((seq_t)candidate))
        {
            if (cache_len == cache_cap)
                grow_seq(&cache, &cache_cap, cache_len + 1);
            cache[cache_len++] = (seq_t)candidate;
        }
        ++residue_idx;
        if (residue_idx == 10)
        {
            residue_idx = 0;
            current_base += 1320;
        }
    }
    return cache[n - 1];
}
