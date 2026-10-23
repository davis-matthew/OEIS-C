/*
 * A175305  a(1)=1. a(n) = the smallest integer > a(n-1) where both a(n)-1 and
 *          a(n)+1 are coprime to all earlier terms of the sequence.
 * https://oeis.org/A175305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses definition: a(1)=1, a(n)=smallest integer > a(n-1) such that
   both a(n)-1 and a(n)+1 are coprime to all earlier terms. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;        /* dynamic array of sequence terms   */
static size_t  seq_cap = 0;      /* allocated capacity                */

/* Ensure the array can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (seq_cap < need) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        seq_cap = new_cap;
    }
}

/* Return the n‑th term of A175305 (1‑based index).  The function may be
   called in any order; previously computed terms are memoised. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, harmless */

    /* Initialise the array on first call. */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq[0] = (seq_t)1;                /* a(1) = 1 */
        /* seq_size is implicitly 1 now; we avoid an explicit counter. */
    }

    /* Expand and generate terms up to index n (1‑based). */
    size_t needed = (size_t)n;
    size_t current = 1;            /* number of terms already stored */
    ensure_capacity(needed);
    while (current < needed) {
        seq_t cand = seq[current - 1] + (seq_t)1;   /* candidate > previous term */
        bool good = false;
        while (!good) {
            good = true;
            for (size_t j = 0; j < current; ++j) {
                if (gcd(cand - (seq_t)1, seq[j]) != (seq_t)1 ||
                    gcd(cand + (seq_t)1, seq[j]) != (seq_t)1) {
                    good = false;
                    break;
                }
            }
            if (!good) ++cand;
        }
        seq[current] = cand;
        ++current;
    }

    return seq[n - 1];
}
