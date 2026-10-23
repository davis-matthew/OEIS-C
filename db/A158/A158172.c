/*
 * A158172  Number of nondecreasing integer sequences of length 38 with sum
 *          zero and sum of absolute values 2n.
 * https://oeis.org/A158172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 43, u64 n <= 138, u128 n <= 222,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A158172
 *  Number of nondecreasing integer sequences of length 38
 *  with sum zero and sum of absolute values 2n.
 *
 *  The formula used here:
 *      a(n) = sum_{p=0}^{38} sum_{q=0}^{38-p} p_p(n) * p_q(n)
 *  where p_k(n) is the number of partitions of n into exactly k parts.
 *
 *  This implementation builds the table of partitions into at most k
 *  parts, Q_k(n), and then derives the exact-part counts.
 *
 *  Author: ChatGPT
 */

#include <stdlib.h>

#ifdef __cplusplus
# include <new>          // placement new for C++ builds
#endif

/* ---- Global data for memoisation ---- */
static seq_t **part_at_most = NULL;   /* part_at_most[k][n] = Q_k(n)          */
static idx_t  max_n = 0;              /* largest n for which the table is built */

/* ------------------------------------------------------------------ */
/*  Allocate a new row of Q_k values up to a given size (inclusive).    */
/*  If C++ is used, the elements are constructed with placement new.     */
/* ------------------------------------------------------------------ */
static seq_t *alloc_row(idx_t size)
{
    seq_t *row = (seq_t *)malloc((size + 1) * sizeof(seq_t));
    if (!row) return NULL;

#ifndef __cplusplus
    /* the memory is already uninitialised; the first assignment will be
     * done by the caller, so nothing else is necessary for C builds.   */
#else
    /* Construct every element in place (default constructor).          */
    for (idx_t i = 0; i <= size; ++i) {
        new (row + i) seq_t();
    }
#endif
    return row;
}

/* ------------------------------------------------------------------ */
/*  Ensure that part_at_most has entries for all n <= needed_n.        */
/*  Grow the table if necessary.                                        */
/* ------------------------------------------------------------------ */
static void ensure_partitions(idx_t needed_n)
{
    if (needed_n <= max_n) return;        /* already computed */

    /* Allocation for first time or expansion */
    if (!part_at_most) {                   /* first allocation */
        part_at_most = (seq_t **)malloc(39 * sizeof(seq_t *));
        if (!part_at_most) return;        /* out of memory */

        for (idx_t k = 0; k <= 38; ++k) {
            part_at_most[k] = alloc_row(needed_n);
            if (!part_at_most[k]) return; /* out of memory */

            /* Q_k(0) = 1 for all k */
            part_at_most[k][0] = (seq_t)1;
        }
    } else {                               /* grow each row */
        for (idx_t k = 0; k <= 38; ++k) {
            seq_t *new_ptr =
                (seq_t *)realloc(part_at_most[k],
                                 (needed_n + 1) * sizeof(seq_t));
            if (!new_ptr) return;          /* out of memory */

            /* Construct newly added elements */
#ifndef __cplusplus
            /* nothing needed for C */
#else
            for (idx_t i = max_n + 1; i <= needed_n; ++i) {
                new (new_ptr + i) seq_t();
            }
#endif
            part_at_most[k] = new_ptr;
        }
    }

    /* Fill in the new values of Q_k(n) */
    for (idx_t n = max_n + 1; n <= needed_n; ++n) {
        for (idx_t k = 0; k <= 38; ++k) {
            if (n == 0) {
                part_at_most[k][n] = (seq_t)1;          /* Q_k(0)=1 */
            } else if (k == 0) {
                part_at_most[k][n] = (seq_t)0;          /* Q_0(n>0)=0 */
            } else {
                seq_t term1 = (n >= k) ? part_at_most[k][n - k] : (seq_t)0;
                seq_t term2 = part_at_most[k - 1][n];
                part_at_most[k][n] = term1 + term2;
            }
        }
    }

    max_n = needed_n;
}

/* ------------------------------------------------------------------ */
/*  Main sequence function                                            */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;             /* values below the offset are undefined */

    ensure_partitions(n);

    /* Compute exact-part counts p_k(n) = Q_k(n) - Q_{k-1}(n)  */
    seq_t exact[39];
    for (idx_t k = 0; k <= 38; ++k) {
        if (k == 0) {
            exact[0] = (n == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            exact[k] = part_at_most[k][n] - part_at_most[k - 1][n];
        }
    }

    /* Sum over all admissible pairs (p,q) with p+q <= 38 */
    seq_t result = (seq_t)0;
    for (idx_t p = 0; p <= 38; ++p) {
        if (exact[p] == (seq_t)0) continue;
        idx_t max_q = 38 - p;
        for (idx_t q = 0; q <= max_q; ++q) {
            if (exact[q] == (seq_t)0) continue;
            result += exact[p] * exact[q];
        }
    }

    return result;
}
