/*
 * A044128  Numbers n such that string 4,2 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044128:
   Numbers n such that the substring “42” occurs in the base‑6
   representation of n but not of n‑1.
   The function is computed by linear search with memoisation. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

/* True if the base‑6 representation of x contains consecutive 4‑followed‑by‑2.
   The check scans the number from least–significant digit upwards. */
static inline bool has_42(seq_t x)
{
    if (x == 0)
        return false;

    seq_t prev = x % 6;   /* least significant digit */
    x /= 6;
    while (x > 0) {
        seq_t cur = x % 6;
        x /= 6;
        if (cur == 4 && prev == 2)
            return true;
        prev = cur;
    }
    return false;
}

/* Return the n‑th term of the sequence (a(1) is the first term). */
seq_t a(idx_t n)
{
    static seq_t *terms = NULL; /* cached terms */
    static size_t len   = 0;    /* number of cached terms */
    static size_t cap   = 0;    /* current allocation capacity */
    static seq_t cur     = 1;    /* next integer to test */

    while (len < (size_t)n) {
        /* Find the next integer that satisfies the condition. */
        while (!(has_42(cur) && !has_42(cur - (seq_t)1)))
            ++cur;

        /* Resize array if necessary. */
        if (len >= cap) {
            size_t new_cap = cap ? cap * 2 : 4;
            if (new_cap <= len)
                while (new_cap <= len)
                    new_cap <<= 1;

            seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
            if (!tmp)          /* allocation failure – give up */
                return (seq_t)0;
            terms = tmp;
            cap   = new_cap;
        }

        /* Store the found term and advance to the next candidate. */
        terms[len++] = cur;
        ++cur;
    }

    return terms[n - 1];
}
