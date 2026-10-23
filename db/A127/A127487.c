/*
 * A127487  Primes p such that 6p - 31 and 6p + 31 are also primes.
 * https://oeis.org/A127487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime_big, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A127487: primes p such that 6p-31 and 6p+31 are also primes.
   The sequence is indexed with offset 1: a(1)=2, a(2)=7, a(3)=13, …   */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* helper functions from the build's utilities */
#include "../utils/is_prime_big.h"   /* bool is_prime_big(seq_t n) */
#include "../utils/seq_alloc.h"     /* seq_t* seq_alloc(size_t n) */
#include "../utils/seq_realloc.h"   /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

/* static cache of found terms */
static seq_t *seq_cache = NULL;   /* array of seq_t */
static size_t  seq_len   = 0;     /* number of terms stored */
static size_t  seq_cap   = 0;     /* allocated capacity of seq_cache */
static seq_t  last_scan  = (seq_t)0;   /* last candidate examined */

/* expand the cache capacity to hold at least `needed` elements */
static void
ensure_cache_size(size_t needed)
{
    if (needed <= seq_cap)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;

    if (seq_cache == NULL) {
        seq_cache = seq_alloc(new_cap);
    } else {
        seq_cache = seq_realloc(seq_cache, seq_cap, new_cap);
    }
    seq_cap = new_cap;
}

/* return the n-th element of the sequence (n >= 1). */
seq_t
a(idx_t n)
{
    if (n < (idx_t)1)          /* undefined for n < 1; return 0 as a guard */
        return (seq_t)0;

    /* extend the cache until it contains at least n elements */
    while (seq_len < (size_t)n) {
        /* advance to the next candidate integer */
        last_scan = last_scan + (seq_t)1;

        /* skip non‑prime candidates */
        if (!is_prime_big(last_scan))
            continue;

        /* special case: p = 2 */
        if (last_scan == (seq_t)2) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = last_scan;
            continue;
        }

        seq_t a1 = (seq_t)6 * last_scan - (seq_t)31;
        seq_t a2 = (seq_t)6 * last_scan + (seq_t)31;

        if (is_prime_big(a1) && is_prime_big(a2)) {
            ensure_cache_size(seq_len + 1);
            seq_cache[seq_len++] = last_scan;
        }
    }

    return seq_cache[(size_t)n - 1];
}
