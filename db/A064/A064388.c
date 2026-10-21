/*
 * A064388  Variation (3) on Recamán's sequence (A005132): set s (the step
 *          size) initially equal to 2; to get a(n), we first try to subtract
 *          s from a(n-1): a(n) = a(n-1)-s if positive and not already in the
 *          sequence, in which case we change s to s+1; if not, a(n) =
 *          a(n-1)+s+i, where i >= 0 is the smallest nu...
 * https://oeis.org/A064388
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for OEIS A064388 (Variation 3 on Recamán's sequence)
   a(1) = 1
   Let s be 2 initially.
   For n>1:
     if a(n-1) > s and (a(n-1)-s) not yet in the sequence,
        a(n) = a(n-1)-s;   s = s+1
     else
        find the smallest i≥0 such that a(n-1)+s+i is new;
        a(n) = a(n-1)+s+i; s = s+i+1
   All values are computed exactly in type seq_t. */

#include <stdlib.h>
#include <stdbool.h>

/* ---- Static data ----------------------------------------------------- */
static seq_t *terms = NULL;      /* array of generated terms */
static size_t terms_cap = 0;     /* allocated capacity of array */
static size_t terms_len = 0;     /* number of terms stored (≥0) */
static idx_t  step_size = 0;     /* current step size s */

/* ---- Helper: grow storage ------------------------------------------- */
static void ensure_terms_capacity(size_t need)
{
    /* Allocate the first chunk lazily. */
    if (terms_cap == 0) {
        terms_cap = 16;
        terms = (seq_t *)malloc(terms_cap * sizeof(seq_t));
    }

    /* Grow if needed. */
    if (need > terms_cap) {
        size_t new_cap = terms_cap * 2;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* ---- Helper: test membership ---------------------------------------- */
static bool seq_contains(seq_t val)
{
    for (size_t i = 0; i < terms_len; ++i)
        if (terms[i] == val)
            return true;
    return false;
}

/* ---- Main function --------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset is 1; n=0 is unused */

    /* Initialise on first call. */
    if (terms_len == 0) {
        ensure_terms_capacity(1);
        terms[0] = (seq_t)1;      /* a(1) */
        terms_len = 1;
        step_size = (idx_t)2;     /* step before a(2) */
    }

    /* Generate terms up to required index. */
    while (terms_len < (size_t)n) {
        seq_t prev = terms[terms_len - 1];
        seq_t next_val = 0;
        bool can_minus = false;

        /* Current step value as seq_t */
        seq_t step_val = (seq_t)step_size;

        /* --- Try to subtract step --- */
        if (prev > step_val) {
            seq_t cand = prev - step_val;
            if (!seq_contains(cand)) {
                next_val = cand;
                step_size = step_size + (idx_t)1;   /* s → s+1 */
                can_minus = true;
            }
        }

        /* --- If subtraction not possible, add --- */
        if (!can_minus) {
            idx_t i = 0;
            seq_t cand = prev + step_val;          /* i = 0 */
            while (seq_contains(cand)) {
                ++i;
                cand = prev + step_val + (seq_t)i;
            }
            next_val = cand;
            step_size = step_size + i + (idx_t)1;   /* s → s + i + 1 */
        }

        /* Store new term. */
        ensure_terms_capacity(terms_len + 1);
        terms[terms_len] = next_val;
        ++terms_len;
    }

    return terms[n - 1];
}
