/*
 * A132425  a(1)=1. a(2)=2. For n >= 3, a(n) is the smallest integer which is
 *          > a(n-1), is not coprime to a(n-1) and is such that a(n)-a(n-1)
 *          does not equal a(m)-a(m-1) for any m < n.
 * https://oeis.org/A132425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Computation of OEIS sequence A132425
 *  a(1)=1, a(2)=2.
 *  For n>=3, a(n) is the smallest integer greater than a(n-1)
 *  that is not coprime to a(n-1) and whose difference a(n)-a(n-1)
 *  has never occurred earlier.
 *
 *  The sequence is generated incrementally; differences are stored
 *  in a boolean array indexed by the difference value (truncated
 *  to size_t; this is safe for the ranges used in the tests).
 */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"

static seq_t *seq = NULL;           /* array of computed terms      */
static size_t seqSize = 0;          /* number of terms stored       */
static size_t seqCap = 0;           /* allocated capacity           */

static bool *usedDiff = NULL;       /* usedDiff[d] == true if d used */
static size_t usedDiffCap = 0;      /* capacity of usedDiff          */

/* Ensure seq array can hold at least 'needed' elements. */
static void ensure_seq_size(size_t needed)
{
    if (seqCap >= needed)
        return;
    size_t newCap = seqCap ? seqCap * 2 : 1;
    while (newCap < needed)
        newCap <<= 1;
    seq_t *newPtr = (seq_t *)malloc(newCap * sizeof(seq_t));
    if (!newPtr) {
        /* Handle allocation failure gracefully: abort. */
        abort();
    }
    for (size_t i = 0; i < seqSize; ++i)
        newPtr[i] = seq[i];
    free(seq);
    seq = newPtr;
    seqCap = newCap;
}

/* Ensure usedDiff array can index differences up to 'diff'. */
static void ensure_used_diff(size_t diff)
{
    if (diff < usedDiffCap)
        return;
    size_t newCap = usedDiffCap ? usedDiffCap * 2 : 1;
    while (newCap <= diff)
        newCap <<= 1;
    bool *newPtr = (bool *)realloc(usedDiff, newCap * sizeof(bool));
    if (!newPtr) {
        abort();
    }
    for (size_t i = usedDiffCap; i < newCap; ++i)
        newPtr[i] = false;
    usedDiff = newPtr;
    usedDiffCap = newCap;
}

/* Generate the sequence up to index 'n' (1‑based). */
static void ensure_seq_upto(idx_t n)
{
    if ((size_t)n <= seqSize)
        return;

    /* Initialise first term if empty. */
    if (seqSize == 0) {
        ensure_seq_size(2);
        seq[0] = 1;
        seqSize = 1;
    }
    /* Initialise second term. */
    if (seqSize == 1 && n >= 2) {
        seq[1] = 2;
        seqSize = 2;
        ensure_used_diff(1);
        usedDiff[1] = true;       /* difference 1 used */
    }

    /* Generate remaining terms. */
    while (seqSize < (size_t)n) {
        seq_t prev = seq[seqSize - 1];
        seq_t candidate = prev + (seq_t)1;

        for (;;) {
            /* must share a factor with previous term */
            if (gcd(prev, candidate) == (seq_t)1) {
                candidate++;
                continue;
            }
            seq_t diff = candidate - prev;
            size_t diffIdx = (size_t)diff;   /* assume diff fits in size_t */

            ensure_used_diff(diffIdx);
            if (usedDiff[diffIdx]) {
                candidate++;
                continue;
            }

            /* Accept candidate */
            ensure_seq_size(seqSize + 1);
            seq[seqSize] = candidate;
            usedDiff[diffIdx] = true;
            seqSize++;
            break;
        }
    }
}

/* Public function required by the framework. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    ensure_seq_upto(n);
    return seq[n - 1];
}
