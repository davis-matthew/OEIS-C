/*
 * A138879  Sum of all parts of the last section of the set of partitions of
 *          n.
 * https://oeis.org/A138879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000041(n)*n - A000041(n-1)*(n-1)
 * Range:     u8 n <= 11, u16 n <= 32, u32 n <= 105, u64 n <= 365,
 *            u128 n <= 1344, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, free */

static seq_t *P = NULL;          /* partition numbers p[0]..p[last] */
static size_t P_alloc = 0;       /* allocated array capacity (in elements) */
static idx_t P_last = 0;         /* highest index already computed (>=0) */

/* Compute partition numbers p[0..n] using the pentagonal‑number recurrence */
static void compute_partitions_upto(idx_t n)
{
    /* Allocate or grow the array if needed */
    size_t needed = (size_t)(n + 1);
    if (needed > P_alloc) {
        size_t new_cap = P_alloc ? P_alloc * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        P = tmp;
        P_alloc = new_cap;
    }

    /* Initialise p[0] on first call */
    if (P_last == 0 && P[0] == (seq_t)0) {
        P[0] = (seq_t)1;   /* p(0) = 1 */
    }

    /* Compute p[i] for i = P_last+1 .. n */
    for (idx_t i = P_last + 1; i <= n; ++i) {
        seq_t pos = 0, neg = 0;   /* sums with + and - signs */

        for (idx_t k = 1; ; ++k) {
            idx_t g1 = (k * (3 * k - 1)) / 2;   /* generalized pentagonal number */
            if (g1 > i) break;

            if (k & 1) pos += P[i - g1];   /* odd k → + */
            else       neg += P[i - g1];   /* even k → - */

            idx_t g2 = (k * (3 * k + 1)) / 2;
            if (g2 > i) continue;

            if (k & 1) pos += P[i - g2];
            else       neg += P[i - g2];
        }

        P[i] = pos - neg;  /* guaranteed non‑negative */
    }

    P_last = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* defined only for n>=1 by OEIS */
    compute_partitions_upto(n);

    seq_t ns = (seq_t)n;            /* cast index to seq_t */
    seq_t term1 = P[n] * ns;
    seq_t term2 = P[n - 1] * (ns - 1);

    return term1 - term2;
}
