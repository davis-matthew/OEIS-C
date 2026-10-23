/*
 * A105099  Nonnegative numbers n such that 23*n^2 + 23*n + 1 = j^2 = a
 *          square.
 * https://oeis.org/A105099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   grow_if_needed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A105099: all n ≥ 0 such that 23*n^2 + 23*n + 1 is a square.
 *
 *  The numbers form two interleaved linear recurrences:
 *      a(n+1) = 2302*a(n) – a(n‑1) + 1150
 *  with the two initial pairs
 *      (a(0),a(1)) = (0,335)   and  (a(0),a(1)) = (0,815).
 *
 *  The global function returns the n‑th value of the sorted union,
 *  where a(1)=0 is the first term.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_if_needed.h"

/* ------------------------------------------------------------------ */
/*  Utility: dynamic array allocation for seq_t                        */
/* ------------------------------------------------------------------ */

static seq_t *seq_malloc(idx_t n) {
    return (seq_t *)malloc((size_t)n * sizeof(seq_t));
}

/* ------------------------------------------------------------------ */

static bool            init          = false;
static seq_t *         A             = NULL;
static size_t          capA          = 0;
static size_t          lenA          = 0;   /* number of elements computed in A */
static idx_t           nextA         = 0;   /* next candidate index in A */

static seq_t *         B             = NULL;
static size_t          capB          = 0;
static size_t          lenB          = 0;
static idx_t           nextB         = 0;

static seq_t *         merged        = NULL;
static size_t          capM          = 0;
static size_t          lenM          = 0;   /* number of merged values */

/* ------------------------------------------------------------------ */
/*  Resize routines                                                    */
/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  Generate next element of sequence A (or B)                         */
/* ------------------------------------------------------------------ */

static void ensure_nextA(void) {
    while (lenA <= nextA) {                      /* need at least one more element */
        grow_if_needed(&capA, &lenA, &A);
        if (lenA < 2) {          /* protect the recurrence for the first 2 elements */
            A[0] = 0;
            A[1] = 335;
            lenA = 2;
            continue;
        }
        seq_t xn = (seq_t)2302 * A[lenA-1] - A[lenA-2] + (seq_t)1150;
        A[lenA++] = xn;
    }
}

static void ensure_nextB(void) {
    while (lenB <= nextB) {
        grow_if_needed(&capB, &lenB, &B);
        if (lenB < 2) {
            B[0] = 0;
            B[1] = 815;
            lenB = 2;
            continue;
        }
        seq_t xn = (seq_t)2302 * B[lenB-1] - B[lenB-2] + (seq_t)1150;
        B[lenB++] = xn;
    }
}

/* ------------------------------------------------------------------ */
/*  Merge step to produce the next sorted value                        */
/* ------------------------------------------------------------------ */

static void grow_merged_if_needed(void) {
    grow_if_needed(&capM, &lenM, &merged);
}

/* ------------------------------------------------------------------ */
/*  Main function:  a(idx_t n)                                         */
/* ------------------------------------------------------------------ */

seq_t a(idx_t n) {
    if (n < 1) return 0;          /* undefined for n<1; return 0 for safety */

    /* first‑time initialization */
    if (!init) {
        init = true;

        /* initial capacity for the sequences */
        capA = 4;  A = seq_malloc(capA);
        capB = 4;  B = seq_malloc(capB);
        capM = 4;  merged = seq_malloc(capM);

        /* seed initial two values for each sequence */
        A[0] = 0;  A[1] = 335;  lenA = 2;
        B[0] = 0;  B[1] = 815;  lenB = 2;

        /* merged list starts empty; its first element will come from the
           merging process below */
        lenM = 0;
        nextA = 0;
        nextB = 0;
    }

    /* extend the merged list until we have at least n elements */
    while (lenM < (size_t)n) {
        /* ensure there are candidates available in both sequences */
        ensure_nextA();
        ensure_nextB();

        /* fetch current candidates */
        seq_t valA = A[nextA];
        seq_t valB = B[nextB];
        seq_t pick;

        if (valA < valB) {
            pick = valA;
            ++nextA;
        } else if (valB < valA) {
            pick = valB;
            ++nextB;
        } else {           /* duplicate values (only 0 at the beginning) */
            pick = valA;     /* or valB */
            ++nextA;
            ++nextB;
        }

        /* store in merged array */
        grow_merged_if_needed();
        merged[lenM++] = pick;
    }

    return merged[n-1];
}
