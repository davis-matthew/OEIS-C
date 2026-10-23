/*
 * A039853  Number of partitions satisfying cn(0,5) + cn(2,5) + cn(3,5) <= 1.
 * https://oeis.org/A039853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n in which the counts of parts
 *            congruent to 0, 2, or 3 modulo 5 sum to at most 1. The code
 *            computes the sequence by dynamic programming: 1. Compute P0[i],
 *            the number of partitions of i using only parts congruent to 1 or
 *            4 (mod 5). 2. Set A[i] = P0[i] (partitions wit...
 * Range:     u8 n <= 24, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *G = NULL;          /* P0 values */
static seq_t *A = NULL;          /* a(n) values */
static size_t  cap = 0;          /* allocated capacity (size_t, 0 initially) */
static idx_t   computed = 0;     /* maximum n computed so far */

/* grow allocation to cover at least (n+1) entries */
static void grow(idx_t n)
{
    if (cap > n) return;                     /* already large enough */

    size_t newcap = (size_t)(n + 1) * 2;      /* grow factor 2 */
    seq_t *tmpG = (seq_t *)realloc(G, newcap * sizeof(seq_t));
    seq_t *tmpA = (seq_t *)realloc(A, newcap * sizeof(seq_t));

    if (!tmpG || !tmpA)                       /* allocation failed */
        exit(EXIT_FAILURE);

    if (tmpG != G) G = tmpG;                  /* update pointers if moved */
    if (tmpA != A) A = tmpA;

    /* initialise new slots to zero */
    for (size_t i = cap; i < newcap; ++i) {
        G[i] = 0;
        A[i] = 0;
    }
    cap = newcap;
}

/* recompute G and A up to 'n' if needed */
static void recompute(idx_t n)
{
    if (computed >= n) return;                /* nothing to do */

    grow(n);                                   /* ensure space */

    /* reset G[0..n] (G[0] = 1, rest 0) */
    G[0] = 1;
    for (idx_t i = 1; i <= n; ++i) G[i] = 0;

    /* unlimited parts ≡ 1 (mod 5) */
    for (idx_t p = 1; p <= n; p += 5) {
        for (idx_t s = p; s <= n; ++s)
            G[s] += G[s - p];
    }

    /* unlimited parts ≡ 4 (mod 5) */
    for (idx_t p = 4; p <= n; p += 5) {
        for (idx_t s = p; s <= n; ++s)
            G[s] += G[s - p];
    }

    /* initialise A with the contributions from G itself (no special part) */
    for (idx_t i = 0; i <= n; ++i) A[i] = G[i];

    /* add contributions from one special part (0, 2 or 3 mod 5) */
    /* parts ≡ 0 (mod 5) : 5,10,15,… */
    for (idx_t p = 5; p <= n; p += 5) {
        for (idx_t s = p; s <= n; ++s)
            A[s] += G[s - p];
    }
    /* parts ≡ 2 (mod 5) : 2,7,12,… */
    for (idx_t p = 2; p <= n; p += 5) {
        for (idx_t s = p; s <= n; ++s)
            A[s] += G[s - p];
    }
    /* parts ≡ 3 (mod 5) : 3,8,13,… */
    for (idx_t p = 3; p <= n; p += 5) {
        for (idx_t s = p; s <= n; ++s)
            A[s] += G[s - p];
    }

    computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                    /* defined only for n≥1, but safe */
    recompute(n);
    return A[n];
}
