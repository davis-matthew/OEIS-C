/*
 * A044247  Numbers n such that string 7,4 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044247
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 1136, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has74
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes: a(n) = the nth positive integer whose base‑8 representation
   contains the substring "74" but whose predecessor does not.
   The function searches sequentially, memoising the found terms in a
   dynamically grown array so that calls with arbitrary n are fast. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has74.h"

/* Static storage for memoisation. */
static struct {
    seq_t *terms;       /* array of found terms */
    size_t len;          /* number of stored terms */
    size_t cap;          /* capacity of the array */
    idx_t next;          /* next integer to test */
} seq_state = { NULL, 0, 0, 1 };

/* Ensure that the terms array has at least @needed capacity. */
static void seq_ensure_capacity(size_t needed)
{
    if (seq_state.cap >= needed)
        return;
    size_t newcap = seq_state.cap ? seq_state.cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newterms = (seq_t *)realloc(seq_state.terms,
                                       newcap * sizeof(seq_t));
    if (!newterms)
        exit(EXIT_FAILURE);            /* out of memory */
    seq_state.terms = newterms;
    seq_state.cap   = newcap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* undefined for offset 1, but safe */

    /* Generate terms until we have at least n of them. */
    while (seq_state.len < (size_t)n) {
        seq_t m = seq_state.next;
        if (has74(m) && !has74(m - 1)) {
            seq_ensure_capacity(seq_state.len + 1);
            seq_state.terms[seq_state.len++] = m;
        }
        seq_state.next++;   /* move to the next candidate */
    }
    return seq_state.terms[n - 1];
}
