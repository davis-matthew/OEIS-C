/*
 * A127752  Row sums of inverse of number triangle A(n,k) = 1/(3n+1) if k <= n
 *          <= 2k, 0 otherwise.
 * https://oeis.org/A127752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2048, u16 n <= 2048, u32 n <= 2048, u64 n <= 2048,
 *            u128 n <= 2048, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; for n>=1, a(n) = (3n+1) - Σ_{k=ceil(n/2)}^{n-1} a(k) */
#include <stdlib.h>
#include <stddef.h>

/* Computes the n-th term of OEIS sequence A127752.
   Uses dynamic programming with prefix sums for O(n) time per call. */
seq_t a(idx_t n)
{
    /* Static cache of previously computed values and their prefix sums */
    static seq_t *A = NULL;       /* computed a(k) values */
    static seq_t *P = NULL;       /* P[i] = Σ_{k=0}^{i-1} a(k) */
    static idx_t cap = 0;         /* capacity of arrays */
    static idx_t used = 0;        /* number of values computed (i.e., used == #A computed) */

    /* First call: allocate minimal capacity */
    if (A == NULL) {
        cap = 1;
        A = (seq_t *)calloc(cap, sizeof(seq_t));
        P = (seq_t *)calloc(cap + 1, sizeof(seq_t));
        if (A == NULL || P == NULL) exit(1); /* abort if allocation fails */
        P[0] = 0;                     /* prefix sum up to index 0 is 0 */
    }

    /* Ensure capacity is sufficient for index n */
    if ((idx_t)(n + 1) > cap) {
        idx_t newcap = cap ? cap : 1;
        while ((idx_t)(n + 1) > newcap) newcap <<= 1;
        seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));
        seq_t *tmpP = (seq_t *)realloc(P, (newcap + 1) * sizeof(seq_t));
        if (tmpA == NULL || tmpP == NULL) exit(1);  /* abort on failure */
        A = tmpA;
        P = tmpP;
        cap = newcap;
    }

    /* Compute values up to n if not already computed */
    for (idx_t i = used; i <= n; ++i) {
        idx_t l = (i + 1) >> 1;              /* ceil(i / 2) */
        seq_t sum = P[i] - P[l];             /* Σ_{k=l}^{i-1} a(k) */
        seq_t val = ((seq_t)i * 3 + 1) - sum;/* a(i) = (3i+1) - sum */
        A[i] = val;
        P[i + 1] = P[i] + val;               /* update prefix sum */
    }
    used = n + 1;                            /* mark values up to n computed */

    return A[n];
}
