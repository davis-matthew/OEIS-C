/*
 * A141799  Number of repeated integer partitions of n.
 * https://oeis.org/A141799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1 * a(n) = ∑_{partitions p of n} ∑_{t∈p} a(t) * (that is,
 *            a(n) equals the total number of parts in all * partitions of n,
 *            with each part weighted by a(t) )
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


/* static memory for partitions (P), sequence values (A)
 * and temporary multiplicity counts (G) */
static seq_t *P = NULL;
static seq_t *A = NULL;
static seq_t *G = NULL;
static size_t cap = 0;           /* current capacity in elements */

/*------------------------------------------------------------*/
/* Ensure that the arrays have capacity at least `needed`.   */
/* New elements are value‑initialised to zero (or default).  */
/*   For C++, use `new`/`delete`; for C use `malloc`/`realloc`. */
/*------------------------------------------------------------*/
static void ensure_capacity(size_t needed)
{
    if (needed <= cap)
        return;
    if (needed < 2)
        needed = 2;

#ifdef __cplusplus
    /* C++: use new[] and delete[] (default constructor applied) */
    seq_t *newP = new seq_t[needed];
    seq_t *newA = new seq_t[needed];
    seq_t *newG = new seq_t[needed];

    /* copy existing data */
    for (size_t i = 0; i < cap && i < needed; ++i) {
        newP[i] = P[i];
        newA[i] = A[i];
        newG[i] = G[i];
    }
    /* zero‑initialise new slots */
    for (size_t i = cap; i < needed; ++i) {
        newP[i] = seq_t();
        newA[i] = seq_t();
        newG[i] = seq_t();
    }

    delete[] P;
    delete[] A;
    delete[] G;
    P = newP;
    A = newA;
    G = newG;
#else
    /* C: use realloc (void* → seq_t*) with cast */
    seq_t *newP = (seq_t*)realloc(P, needed * sizeof(seq_t));
    seq_t *newA = (seq_t*)realloc(A, needed * sizeof(seq_t));
    seq_t *newG = (seq_t*)realloc(G, needed * sizeof(seq_t));
    if (!newP || !newA || !newG) {
        /* allocation failed – terminate */
        exit(1);
    }
    /* zero‑initialise new slots */
    for (size_t i = cap; i < needed; ++i) {
        newP[i] = 0;
        newA[i] = 0;
        newG[i] = 0;
    }

    P = newP;
    A = newA;
    G = newG;
#endif

    cap = needed;
}

/*------------------------------------------------------------*/
/* Compute integer partition numbers p[0..max]                */
/* using the classic O(max^2) dynamic programming algorithm */
/*------------------------------------------------------------*/
static void compute_partitions(idx_t max)
{
    ensure_capacity((size_t)max + 1);

    /* initialise */
    for (idx_t i = 0; i <= max; ++i)
        P[i] = 0;
    P[0] = 1;

    for (idx_t i = 1; i <= max; ++i) {
        for (idx_t s = i; s <= max; ++s) {
            P[s] += P[s - i];
        }
    }
}

/*------------------------------------------------------------*/
/* Compute a[1..max] using the recurrence with multiplicity  */
/*------------------------------------------------------------*/
static void compute_sequence(idx_t max)
{
    if (max == 0)
        return;

    ensure_capacity((size_t)max + 1);

    /* base values: a[1] = 1, the rest start at 1 (each partition's unit part) */
    if (max >= 1)
        A[1] = 1;
    for (idx_t n = 2; n <= max; ++n)
        A[n] = 1;

    /* ensure partitions up to max are available */
    compute_partitions(max);

    /* for each part size k, count its multiplicity among partitions of all n > k */
    for (idx_t k = 1; k <= max - 1; ++k) {
        /* compute the multiplicity array G for this k */
        for (idx_t n = k; n <= max; ++n) {
            seq_t val = P[n - k];
            if (n - k >= k)
                val += G[n - k];
            G[n] = val;
        }
        /* accumulate contribution to A[n] (n > k) */
        seq_t ak = A[k];
        for (idx_t n = k + 1; n <= max; ++n) {
            A[n] += ak * G[n];
        }
    }
}

/*------------------------------------------------------------*/
/* Public function required by the exercise                  */
/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* compute up to n (calls to compute_sequence will handle larger n later) */
    compute_sequence(n);
    return A[n];
}
