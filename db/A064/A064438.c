/*
 * A064438  Numbers which are divisible by the sum of their quaternary digits.
 * https://oeis.org/A064438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 83, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   quaternary_digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A064438 – numbers divisible by the sum of their quaternary digits.
 *
 *  a(n) = the n-th positive integer k ≥ 1 for which
 *         (sum of the base‑4 digits of k) divides k.
 *
 *  The function is memoised: subsequent calls only enumerate new
 *  candidates.  All arithmetic is done in the type seq_t; the code
 *  works for arbitrary‑precision builds as well as fixed‑width
 *  integers.  No floating point or external helpers are used.
 */

#include <stdlib.h>
#include "../utils/quaternary_digit_sum.h"

static seq_t *seq_cache   = NULL;  /* cached terms a(1), a(2), …          */
static size_t seq_cache_len  = 0;   /* number of cached terms              */
static size_t seq_cache_cap  = 0;   /* storage capacity                    */
static seq_t   seq_last      = 0;   /* last integer that was examined      */

/* Ensure that the cache can hold at least 'min_needed' terms.  */
static void seq_grow_cache(size_t min_needed)
{
    if (seq_cache_cap >= min_needed) return;
    size_t new_cap = seq_cache_cap ? seq_cache_cap << 1 : 128;
    while (new_cap < min_needed) new_cap <<= 1;
    seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    seq_cache_cap = new_cap;
}

/* Append a new term to the cache; caller must guarantee enough space. */
static void seq_append(seq_t val)
{
    seq_cache[seq_cache_len++] = val;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n = 0 */

    /* Return the term if we already have it cached. */
    if (seq_cache_len >= (size_t)n) return seq_cache[n - 1];

    /* Enumerate candidates until we produce the n‑th term. */
    seq_t cur = seq_last + 1;

    for (;;) {
        seq_t sum = quaternary_digit_sum(cur);
        if (sum != 0 && cur % sum == 0) {
            seq_grow_cache(seq_cache_len + 1);
            seq_append(cur);
            seq_last = cur;          /* update last examined */
            if (seq_cache_len >= (size_t)n) break;
        }
        ++cur;
    }
    return seq_cache[n - 1];
}
