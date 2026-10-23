/*
 * A140038  Primes of the form 24x^2+24xy+83y^2.
 * https://oeis.org/A140038
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 202, u32 n <= 40, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A140038 – primes of the form 24x² + 24xy + 83y².
 *
 *  A prime p occurs in the sequence iff
 *      p ≡ 83, 131, 227, 299, 395, 563, 635, 755,
 *        899, 923, 1091, 1139, 1403, 1427, 1811  (mod 1848).
 *
 *  The routine generates primes in ascending order by examining
 *  the 15 admissible residue classes modulo 1848. The search
 *  continues until the requested index n is reached, caching all
 *  intermediate terms so that subsequent calls are fast.
 *
 *  The code works for any unsigned integer type assigned to seq_t
 *  (uint8_t … uint64_t, unsigned __int128, or a bignum class in C++).
 *  It uses is_prime_big(seq_t) from the utils library for primality testing.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

 /* Admissible residues modulo 1848 – 15 values */
static const unsigned int residues[] = {
    83u, 131u, 227u, 299u, 395u, 563u,
    635u, 755u, 899u, 923u, 1091u, 1139u,
    1403u, 1427u, 1811u
};
#define RES_CNT   (sizeof(residues)/sizeof(residues[0]))
#define MODULO    1848u

/* Static state to allow memoisation across calls */
static bool          init_done   = false;
static idx_t         cur_k       = 0;   /* current block k      */
static size_t        cur_r       = 0;   /* current residue index */
static seq_t        *cache       = NULL; /* memoised terms       */
static size_t        cache_size  = 0;   /* how many terms cached*/
static size_t        cache_cap   = 0;   /* allocated capacity   */

/* Ensure the cache can hold up to 'need' terms. */
static void ensure_cache_cap(size_t need)
{
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp && new_cap > 0) return; /* allocation failure – caller will see 0 */
    cache       = tmp;
    cache_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0; /* undefined – keep safe */

    /* Initialise static state on entry. */
    if (!init_done) {
        init_done = true;
        cur_k     = 0;
        cur_r     = 0;
    }

    /* Grow cache if it currently cannot hold n terms. */
    ensure_cache_cap((size_t)n);

    /* Already computed? */
    if (cache_size >= (size_t)n) return cache[n - 1];

    /* Generate terms until we have the n‑th one. */
    while (cache_size < (size_t)n) {
        /* Candidate in the current residue class. */
        seq_t candidate = (seq_t)MODULO * (seq_t)cur_k + (seq_t)residues[cur_r];

        /* Test for primality. */
        if (is_prime_big(candidate))
            cache[cache_size++] = candidate;

        /* Advance to next residue class (or next block). */
        ++cur_r;
        if (cur_r == RES_CNT) {
            cur_r = 0;
            ++cur_k;
        }
    }

    return cache[n - 1];
}
