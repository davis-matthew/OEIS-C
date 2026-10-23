/*
 * A109888  Sequence with properties that a(1)=1, no prime appears in the
 *          sequence or the first differences and all other integers exactly
 *          once (in sequence or first differences).
 * https://oeis.org/A109888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A109888
 *  
 *  a(1) = 1.
 *  For n > 1, let d be the smallest positive integer that
 *  (i) has not yet appeared either as a term or as a difference,
 *  (ii) is not prime, and
 *  (iii) a(n‑1)+d is not prime and has not yet appeared.
 *  Then a(n) = a(n‑1)+d.
 *  Every non‑prime integer (and 1) appears exactly once, either as a
 *  term or as a difference; no prime ever appears in the sequence
 *  or in the first differences.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"

/* Static storage for memoised terms */
static seq_t *seq      = NULL;
static idx_t seq_cap   = 0;   /* allocated capacity of seq */
static idx_t seq_size  = 0;   /* number of terms computed */

/* Static bitmap for numbers that have already appeared */
static bool *used      = NULL;
static idx_t used_cap  = 0;   /* maximum index (inclusive) stored in used */

/* Ensure seq has at least min_cap elements.  */
static void ensure_seq_cap(idx_t min_cap)
{
    if (min_cap <= seq_cap)
        return;
    idx_t new_cap = seq_cap ? seq_cap : 16;
    while (new_cap < min_cap)
        new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(seq, (size_t)new_cap * sizeof(seq_t));
    if (!tmp)
        return;          /* allocation failed – leave seq unchanged */
    seq = tmp;
    seq_cap = new_cap;
}

/* Ensure used can index up to min_cap (inclusive). */
static void ensure_used_cap(idx_t min_cap)
{
    if (min_cap <= used_cap)
        return;
    idx_t new_cap = used_cap ? used_cap : 16;
    while (new_cap <= min_cap)
        new_cap <<= 1;

    bool *tmp = (bool *)realloc(used, (size_t)new_cap * sizeof(bool));
    if (!tmp)
        return;          /* allocation failed – leave used unchanged */
    /* Initialise new slots */
    for (idx_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}

/* Mark integer x as used (either as a term or a difference). */
static void mark_used(idx_t x)
{
    ensure_used_cap(x);
    used[x] = true;
}

/* Return true if integer x has already been used. */
static inline bool is_used(idx_t x)
{
    return (x < used_cap) && used[x];
}

/* Compute a(n).  Uses static memoisation so subsequent calls are fast. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;            /* undefined by OEIS – return 0 for safety */

    /* Initialise first term if not yet done */
    if (!seq_size) {
        ensure_seq_cap(1);
        seq[0] = 1;          /* a(1) = 1 */
        seq_size = 1;
        mark_used(1);        /* 1 appears as a term */
    }

    /* Compute terms up to index n if necessary */
    while (seq_size < n) {
        seq_t   prev   = seq[seq_size - 1];
        idx_t   prev_i = (idx_t)prev;  /* work in a wider type */

        idx_t d = 1;  /* candidate difference */
        for (;;) {
            /* Skip used numbers */
            if (is_used(d))
                ++d;
            /* Skip primes */
            else if (is_prime((uint64_t)d))
                ++d;
            else {
                idx_t sum_i = prev_i + d;
                /* Skip if the sum was already used */
                if (is_used(sum_i))
                    ++d;
                /* Skip if the sum is prime */
                else if (is_prime((uint64_t)sum_i))
                    ++d;
                else
                    break;   /* found admissible d */
            }
        }

        /* Record the new term */
        idx_t new_term_i = prev_i + d;
        seq_t new_term   = (seq_t)new_term_i;  /* may truncate if too large;
                                                * but caller will only use
                                                * terms that fit seq_t */
        ensure_seq_cap(seq_size + 1);
        seq[seq_size++] = new_term;

        /* Mark the difference and the new term as used */
        mark_used(d);
        mark_used(new_term_i);
    }

    return seq[n - 1];
}
