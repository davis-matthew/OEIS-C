/*
 * A052862  Expansion of e.g.f. log(-1/(-2+exp(x)))*x.
 * https://oeis.org/A052862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 0, a(2) = 2; For n ≥ 3: a(n) = 2 * n *
 *            A000670(n‑2) where A000670(k) is the k‑th ordered Bell number
 *            (number of preferential arrangements)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>    /* malloc, free, realloc */

/*  Static cache for the ordered Bell numbers A000670(k).  */
static seq_t *A000 = NULL;   /* array of size capA, valid indices 0..maxA-1 */
static idx_t  maxA = 0;      /* current maximum k for which A000[k] is valid */
static size_t capA = 0;      /* allocated array capacity */

/*  Ensure that A000 contains values for indices 0 .. target (inclusive).  */
static void ensure_A000(idx_t target)
{
    if (target < maxA)          /* already plenty of cached values */
        return;

    /* Expand the array if necessary. */
    if ((size_t)target + 1 > capA)
    {
        size_t new_cap = (size_t)target + 1;
        seq_t *newA = (seq_t *)realloc(A000, new_cap * sizeof(seq_t));
        if (!newA)                 /* allocation failed: leave state unchanged */
            return;
        A000 = newA;
        capA = new_cap;
    }

    /* Compute missing values of A000 from maxA up to target.  */
    for (idx_t i = maxA; i <= target; ++i)
    {
        if (i == 0)         /* A000670(0) = 1 by convention */
        {
            A000[0] = (seq_t)1;
            continue;
        }

        seq_t sum   = (seq_t)0;
        seq_t binom = (seq_t)1;      /* C(i,0) */

        for (idx_t k = 0; k < i; ++k)
        {
            sum = sum + binom * A000[k];
            if (k + 1 < i)
            {
                seq_t prod = binom * (seq_t)(i - k);
                binom = prod / (seq_t)(k + 1);   /* exact division */
            }
        }
        A000[i] = sum;
    }

    maxA = target + 1;   /* we now have values up to target inclusive */
}

/*  Return the n-th term of sequence A052862.  */
seq_t a(idx_t n)
{
    /* Base cases from the OEIS definition. */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)2;

    /* For n ≥ 3 use the recurrence involving ordered Bell numbers. */
    idx_t k = n - 2;
    ensure_A000(k);                /* must have A000[k] available */

    seq_t n_val = (seq_t)n;
    seq_t term  = (seq_t)2 * n_val * A000[k];
    return term;
}
