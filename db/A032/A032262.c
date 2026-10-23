/*
 * A032262  Number of ways to partition n labeled elements into pie slices
 *          allowing the pie to be turned over.
 * https://oeis.org/A032262
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A032262: a(n) = 2^(n-2) + A000670(n-1) for n ≥ 2
 * (A000670 are the Fubini (ordered Bell) numbers.
 *  a(0) = 1, a(1) = 1.
 *
 *  The function uses the recurrence
 *      F(0) = 1
 *      F(n) = Σ_{k=0}^{n-1} C(n,k) · F(k)   (n ≥ 1)
 *  to compute F(n-1).  Internally it memoises computed values in a
 *  dynamic array so that calls to a() are fast even if the indices
 *  are given in arbitrary order.
 */

#include <stdlib.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t exp); */

static seq_t *fub = NULL;          /* memoised Fubini numbers */
static size_t fub_size   = 0;      /* capacity of the array   */
static size_t fub_valid  = 0;      /* number of elements
                                      that have a valid value
                                      (last valid index + 1) */

static void ensure_fubini(idx_t upto)
{
    /* Initialise the table if it does not exist yet. */
    if (fub == NULL) {
        fub_size = 1;
        fub = (seq_t *)malloc(fub_size * sizeof(seq_t));
        if (!fub) exit(EXIT_FAILURE);
        fub[0] = (seq_t)1;          /* F(0) = 1 */
        fub_valid = 1;
    }

    /* Grow the buffer if required. */
    if ((size_t)upto >= fub_size) {
        size_t new_cap = fub_size ? fub_size : 1;
        while (new_cap <= (size_t)upto) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(fub, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        fub = tmp;
        fub_size = new_cap;
    }

    /* Compute the missing Fubini numbers. */
    for (idx_t n = fub_valid; n <= upto; ++n) {
        /* Recurrence: F(n) = Σ_{k=0}^{n-1} C(n,k)·F(k)   (n≥1) */
        seq_t sum = 0;
        if (n >= 1) {
            seq_t binom = 1;                  /* C(n,0) = 1 */
            for (idx_t k = 0; k < n; ++k) {
                sum += binom * fub[(size_t)k];
                /* Update binom to C(n, k+1) */
                binom = binom * (seq_t)(n - k) / (seq_t)(k + 1);
            }
        }
        fub[(size_t)n] = sum;
    }
    fub_valid = (size_t)upto + 1;
}

seq_t a(idx_t n)
{
    if (n == 0 || n == 1) {
        return (seq_t)1;
    }

    /* n ≥ 2 : compute 2^(n-2) + F(n-1) */
    ensure_fubini(n - 1);                 /* ensure F(n-1) is ready */
    seq_t part1 = pow2(n - 2);              /* 2^(n-2) */
    seq_t part2 = fub[(size_t)(n - 1)];    /* F(n-1) */
    return part1 + part2;
}
