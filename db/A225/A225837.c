/*
 * A225837  Numbers of form 2^i*3^j*(6k+1), i, j, k >= 0.
 * https://oeis.org/A225837
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 140, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A225837: the nth positive integer of the form 2^i * 3^j * (6k+1)
   (i, j, k ≥ 0).  The sequence is generated lazily; previously found terms
   are kept in a cache so subsequent calls are fast. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ----- cache for the generated terms ----- */
static seq_t *seq_cache = NULL;   /* array of sequence values      */
static size_t seq_len  = 0;        /* number of values stored       */
static size_t seq_cap  = 0;        /* allocated capacity           */
static idx_t  next_candidate = 1;  /* next integer to test in order */

/* append a value to the cache, expanding it if necessary */
static void cache_append(seq_t value)
{
    if (seq_len == seq_cap) {
        size_t new_cap = seq_cap == 0 ? 16 : seq_cap * 2;
        seq_cache = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!seq_cache) abort();  /* out‑of‑memory */
        seq_cap = new_cap;
    }
    seq_cache[seq_len++] = value;
}

/* test whether x has only the allowed prime factors (2, 3, and any number
   ≡ 1 (mod 6) ).  Equivalent: after removing all factors 2 and 3,
   the remaining part must be congruent to 1 modulo 6. */
static bool in_sequence(seq_t x)
{
    seq_t t = x;
    /* strip factors 2 */
    while ((t % 2) == 0) t /= 2;
    /* strip factors 3 */
    while ((t % 3) == 0) t /= 3;
    /* the remaining part must be ≡ 1 (mod 6) */
    return ((t % 6) == 1);
}

/* ----- main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined; safe default */

    /* produce terms until we have at least n of them */
    while (seq_len < n) {
        if (in_sequence((seq_t)next_candidate))
            cache_append((seq_t)next_candidate);
        ++next_candidate;
    }
    return seq_cache[n - 1];
}
