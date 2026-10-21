/*
 * A054033  Numbers n such that n^2 contains exactly 5 different digits.
 * https://oeis.org/A054033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_five_distinct_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A054033: a(n) is the n-th positive integer whose square contains
   exactly five distinct decimal digits.
   (Computed by brute‑force search.) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_five_distinct_digits.h"

/* Dynamic array of the sequence values. */
static seq_t *vals      = NULL;   /* storage for terms                 */
static size_t vals_len  = 0;      /* number of terms stored           */
static size_t vals_cap  = 0;      /* current capacity                  */

/* Next number to test and how many terms have been collected so far. */
static idx_t next_candidate = 1;
static idx_t collected     = 0;

/* Ensure there is space for an additional value in vals. */
static void ensure_vals_capacity(void)
{
    if (vals_len == vals_cap)
    {
        size_t new_cap = vals_cap ? vals_cap * 2 : 128;
        seq_t *tmp = (seq_t *)realloc(vals, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory */
        vals = tmp;
        vals_cap = new_cap;
    }
}

/* Generate terms up to the target index (1‑based). */
static void generate_up_to(idx_t target)
{
    while (collected < target)
    {
        seq_t cand   = (seq_t)next_candidate;
        unsigned __int128 sq = (unsigned __int128)cand * (unsigned __int128)cand;

        if (has_five_distinct_digits(sq))
        {
            ensure_vals_capacity();
            vals[vals_len++] = cand;
            ++collected;
        }
        ++next_candidate;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* undefined for n=0, safe fallback */
    if (n > collected) generate_up_to(n);
    return vals[n - 1];
}
