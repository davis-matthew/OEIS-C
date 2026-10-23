/*
 * A044489  Numbers n such that string 1,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer whose base‑6 representation
 *            contains the adjacent digits 1,0, but the next integer (n+1)
 *            does NOT contain that substring. Computed by brute‑force search
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   has_10_in_base6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_10_in_base6.h"

/* ---- dynamic array for cached terms ---- */
static seq_t *terms = NULL;   /* array of found terms */
static idx_t   size  = 0;     /* number of terms stored */
static idx_t   cap   = 0;     /* current capacity of the array */
static seq_t   next_candidate = 0;  /* next integer to test */

/* Ensure the array can hold at least 'need' elements. */
static void ensure_capacity(idx_t need)
{
    if (cap >= need) return;
    idx_t newcap = cap ? cap * 2 : 16;
    while (newcap < need) newcap *= 2;
    terms = (seq_t *)realloc(terms, (size_t)newcap * sizeof(seq_t));
    cap   = newcap;
}

/* Generate and store terms until at least 'upto' terms are available. */
static void generate_terms(idx_t upto)
{
    while (size < upto) {
        next_candidate++;  /* start testing from 1 upwards */
        if (has_10_in_base6(next_candidate) &&
            !has_10_in_base6(next_candidate + 1))
        {
            ensure_capacity(size + 1);
            terms[size++] = next_candidate;
        }
    }
}

/* Public interface required by the build system. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, return 0 safely */
    generate_terms(n);            /* fill cache up to n-th term */
    return terms[n - 1];          /* 1‑based indexing */
}
