/*
 * A044513  Numbers n such that string 5,0 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Validated: partial, pass, 43 terms through n = 43 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: a(n) is the nth integer n such that its base‑6 representation
 * contains the substring "5,0" but the integer n+1 does not. */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility functions provided by the build, used for dynamic array handling */
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

static seq_t   *terms   = NULL; /* array of found terms */
static size_t   terms_cap = 0;  /* current capacity of the array */
static size_t   terms_n   = 0;  /* number of terms stored */
static seq_t   last_checked = (seq_t)0; /* last integer examined */

/* Return true iff the base‑6 representation of x contains the substring 5,0. */
static bool has50(seq_t x)
{
    int prev = -1;           /* previous digit, -1 means none yet */
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)6);   /* current least significant digit */
        if (prev == 0 && d == 5)       /* pattern 5,0 detected (5 is more significant) */
            return true;
        prev = d;
        x = x / (seq_t)6;
    }
    return false;
}

/* Return true iff x satisfies the OEIS condition. */
static bool matches(seq_t x)
{
    return has50(x) && !has50(x + (seq_t)1);
}

/* Ensure the dynamic array has at least 'needed' capacity. */
static void ensure_capacity(size_t needed)
{
    if (needed <= terms_cap) return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = seq_realloc(terms, terms_cap, new_cap);
    terms = tmp;
    terms_cap = new_cap;
}

/* Main function requested by the testing harness. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for offset 1, return 0 anyway */

    /* Generate terms until we have at least n terms. */
    while (terms_n < (size_t)n) {
        last_checked = last_checked + (seq_t)1;
        if (matches(last_checked)) {
            ensure_capacity(terms_n + 1);
            terms[terms_n++] = last_checked;
        }
    }
    return terms[n - 1];
}
