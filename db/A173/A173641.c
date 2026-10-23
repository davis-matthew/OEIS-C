/*
 * A173641  Primes p such that p^2+4 and p^2-6 are both prime.
 * https://oeis.org/A173641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 279, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A173641: primes p such that p^2+4 and p^2-6 are both prime.
 * The function builds the sequence by searching for primes incrementally
 * and testing the two primality conditions with isPrimeGeneric.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

/* cache of already found terms */
static seq_t  *seq_cache = NULL;      /* array of found primes */
static size_t  cache_len   = 0;       /* number of cached terms   */
static size_t  cache_cap   = 0;       /* allocated capacity       */

/* progress of the search */
static seq_t   next_cand = (seq_t)1;  /* current candidate (will be incremented to 3) */

static void ensure_cache_cap(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq_cache = tmp;
    cache_cap = new_cap;
}

/* extend cache until at least n terms are available */
static void ensure_until(idx_t n)
{
    if (n == 0) return;          /* sequence is undefined for n = 0, but harmless */

    while (cache_len < n) {
        /* next odd candidate */
        next_cand += (seq_t)2;
        if (next_cand <= (seq_t)1) next_cand = (seq_t)3;  /* safety for very small seq_t */

        /* candidate must be prime */
        if (!isPrimeGeneric(next_cand)) continue;

        /* check the two derived primes */
        seq_t sq = next_cand * next_cand;
        seq_t v1 = sq + (seq_t)4;
        seq_t v2 = sq - (seq_t)6;
        if (isPrimeGeneric(v1) && isPrimeGeneric(v2)) {
            ensure_cache_cap(cache_len + 1);
            seq_cache[cache_len++] = next_cand;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined, but harmless */
    ensure_until(n);
    return seq_cache[n - 1];
}
