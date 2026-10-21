/*
 * A099356  From P-positions in a certain game. Pile 1 tokens for select
 *          function x1*[1+(-)^x1]+1.
 * https://oeis.org/A099356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = smallest non‑negative integer not appearing among
 *            {a(0),b(0),a(1),b(1),…,a(n-1),b(n-1)}. b(0)=0, a(0)=0. For n≥1:
 *            a(n) = mex of the values seen so far, b(n) = b(n-1) +
 *            (-1)^{a(n-1)}·a(n-1) + a(n) + 1
 * Range:     u8 n <= 47, u16 n <= 699, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation memoises all values up to the requested n.
   It uses a simple vector of all used numbers (both a‑ and b‑values)
   for lookup of the next mex.  The vector grows by doubling the
   capacity.  All arithmetic is performed with seq_t, which may be
   an unsigned integer type or an arbitrary‑precision integer class. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *A = NULL;           /* a(n) values */
static seq_t *B = NULL;           /* b(n) values */
static size_t A_cap = 0, B_cap = 0;
static idx_t CurrentN = 0;       /* highest n computed (≥0) */

/* All numbers that have appeared so far (both a and b).  They are
   kept in an unsorted vector for simplicity.  The vector grows as needed
   and duplicates are simply inserted again.  This does not affect the
   correctness of mex computation. */
static seq_t *Used = NULL;
static size_t Used_len = 0, Used_cap = 0;

/* Ensure the capacity of an array of seq_t is at least (needed + 1). */
static void ensure_capacity(seq_t **arr, size_t *cap, idx_t needed)
{
    if (needed + 1 > *cap) {
        size_t new_cap = *cap ? (*cap << 1) : 1;
        while (new_cap <= (size_t)needed + 1) new_cap <<= 1;
        *arr = seq_realloc(*arr, *cap, new_cap);
        *cap = new_cap;
    }
}

/* Ensure A and B arrays can hold up to index n. */
static void ensure_AB(idx_t n)
{
    ensure_capacity(&A, &A_cap, n);
    ensure_capacity(&B, &B_cap, n);
}

/* Ensure the Used vector can hold at least n+1 elements. */
static void ensure_used(idx_t n)
{
    if ((size_t)n + 1 > Used_cap) {
        size_t new_cap = Used_cap ? (Used_cap << 1) : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        Used = seq_realloc(Used, Used_cap, new_cap);
        Used_cap = new_cap;
    }
}

/* Linear search for a value in the Used vector. */
static bool used_contains(seq_t v)
{
    for (size_t i = 0; i < Used_len; ++i) {
        if (Used[i] == v) return true;
    }
    return false;
}

/* Insert a value into the Used vector (duplicates allowed). */
static void used_insert(seq_t v)
{
    ensure_used(Used_len);
    Used[Used_len++] = v;
}

/* Computes a(n).  The function is memoised and works in any order of
   calls. */
seq_t a(idx_t n)
{
    /* Initialise on the very first call. */
    if (CurrentN == 0 && A == NULL) {
        ensure_AB(0);
        A[0] = (seq_t)0;
        B[0] = (seq_t)0;
        used_insert((seq_t)0);
        CurrentN = 0;
    }

    if (n > CurrentN) {
        seq_t cur_mex = (seq_t)0;

        /* Advance cur_mex to the smallest unused integer.  Since
           a(0)=0 and it is already used, cur_mex will start at 1. */
        while (used_contains(cur_mex)) cur_mex = cur_mex + (seq_t)1;

        for (idx_t i = CurrentN + 1; i <= n; ++i) {
            /* a(i) = cur_mex (the current mex). */
            seq_t ai = cur_mex;
            used_insert(ai);
            cur_mex = cur_mex + (seq_t)1;
            while (used_contains(cur_mex)) cur_mex = cur_mex + (seq_t)1;

            /* Compute b(i). */
            seq_t prev_a = A[i - 1];
            seq_t prev_b = B[i - 1];
            seq_t bi = prev_b;
            if (prev_a & (seq_t)1) {          /* a(i-1) odd: subtract */
                bi = bi - prev_a;
            } else {                          /* a(i-1) even: add */
                bi = bi + prev_a;
            }
            bi = bi + ai + (seq_t)1;

            /* Store and record the new values. */
            ensure_AB(i);
            A[i] = ai;
            B[i] = bi;
            used_insert(bi);
        }
        CurrentN = n;
    }

    return A[n];
}
