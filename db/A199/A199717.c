/*
 * A199717  Numbers k such that 6*k-1 is composite, but 6*k-5 is prime.
 * https://oeis.org/A199717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A199717: numbers k such that 6*k-1 is composite but 6*k-5 is prime.
   Computed by linear search with memoisation. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static seq_t *seq_cache = NULL;   /* cached k-values, 0‑based          */
static idx_t  seq_len   = 0;      /* number of cached values          */
static idx_t  seq_cap   = 0;      /* current capacity of seq_cache    */
static idx_t  k_next    = 0;      /* last tested k                    */

/* Grow the cache to hold at least 'needed' elements. */
static void
ensure_capacity(size_t needed)
{
    if (seq_cap >= needed)
        return;

    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();
    seq_cache = new_cache;
    seq_cap   = new_cap;
}

/* Return the n‑th element of the sequence (a(1)=6).  Undefined for n<1. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                /* undefined for n<1 (offset 1) */

    /* Extend cache until we have at least n values. */
    while (seq_len < n) {
        ++k_next;                /* next candidate k           */
        seq_t m2 = (seq_t)6 * k_next - 5;   /* 6*k-5  (prime test)   */
        seq_t m1 = (seq_t)6 * k_next - 1;   /* 6*k-1  (composite test) */

        /* 6*k-5 must be prime and 6*k-1 composite. */
        if (is_prime_big(m2) && !is_prime_big(m1)) {
            ensure_capacity(seq_len + 1);
            seq_cache[seq_len] = k_next;
            ++seq_len;
        }
    }

    return seq_cache[n - 1];
}
