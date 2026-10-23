/*
 * A026671  Number of lattice paths from (0,0) to (n,n) with steps (0,1),
 *          (1,0) and, when on the diagonal, (1,1).
 * https://oeis.org/A026671
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = binomial(2*n,n) + Σ_{i=1..n}
 *            a(i-1)*binomial(2*(n-i),n-i), a(0) = 1. Uses the recurrence in
 *            the OEIS entry
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* See the problem statement for the meanings of seq_t and idx_t. */
static seq_t *A = NULL;   /* a(n) values */
static seq_t *D = NULL;   /* D[n] = binomial(2*n,n) */
static size_t  cap = 0;   /* allocated capacity (number of slots) */
static idx_t   last = (idx_t)-1; /* last index computed */


/* Ensure that the cache contains values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* 1. Allocate or grow storage if required. */
    if (cap <= (size_t)n) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;

        seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        seq_t *tmpD = (seq_t *)realloc(D, newcap * sizeof(seq_t));

        if (!tmpA || !tmpD) abort();   /* OOM – abort is acceptable in this context */

        A = tmpA;
        D = tmpD;
        cap = newcap;
    }

    /* 2. Compute missing central binomials D[k] = C(2k,k). */
    if (last == (idx_t)-1) {       /* first run – initialise D[0] and A[0] */
        D[0] = (seq_t)1;
        A[0] = (seq_t)1;
        last = 0;
    }

    for (idx_t k = last + 1; k <= n; ++k) {
        /* D[k] = D[k-1] * (4k-2) / k */
        seq_t prev = D[k - 1];
        seq_t mult = (seq_t)(4 * k - 2);
        D[k] = (prev * mult) / (seq_t)k;
    }

    /* 3. Compute missing a(k) values using the recurrence. */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k == 0) continue;                 /* already set */
        seq_t sum = (seq_t)0;
        for (idx_t j = 0; j < k; ++j) {
            sum += A[j] * D[k - 1 - j];
        }
        A[k] = D[k] + sum;
    }

    last = n;
}

/* The public sequence function. */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return A[n];
}
