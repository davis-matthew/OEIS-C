/*
 * A126461  Column 0 of triangle A126460; equals the number of subpartitions
 *          of the partition {(k^2 + 3*k - 4)*k/6, k>=0}.
 * https://oeis.org/A126461
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 - ∑_{k=0}^{n-1} (-1)^{n-k} * a(k) * C(p(k), n-k), where
 *            * p(k) = ((k*k + 3*k - 4)*k)/6. C is the binomial coefficient. *
 *            This implements the recurrence described by the generating
 *            function
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/* Forward declaration of the function required by the test harness. */
seq_t a(idx_t n);

/* -------------------------------------------------------------------- */
/*            static storage and helper functions                      */
/* -------------------------------------------------------------------- */
static seq_t *A = NULL;          /* memoised a(k) values */
static seq_t *P = NULL;          /* memoised p(k) values   */
static seq_t *B = NULL;          /* current binomial values C(p(k), n-k) for the current n */
static size_t    CAP = 0;         /* allocated capacity for the arrays */
static idx_t     COMPUTED = 0;    /* number of a values computed (0 means none) */

/* enlarge capacity to at least n+1 */
static void grow_capacity(idx_t n)
{
    if (CAP > (size_t)n)
        return;

    size_t new_cap = CAP ? (CAP << 1) : 1U;
    while (new_cap <= (size_t)n)
        new_cap <<= 1U;

    seq_t *newA = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
    seq_t *newP = (seq_t *)realloc(P, new_cap * sizeof(seq_t));
    seq_t *newB = (seq_t *)realloc(B, new_cap * sizeof(seq_t));

    /* If realloc failed, leave the old pointers intact (the build
       environment guarantees enough memory). */
    if (newA) A = newA;
    if (newP) P = newP;
    if (newB) B = newB;

    /* initialise new entries */
    for (size_t i = CAP; i < new_cap; ++i) {
        uint64_t pk = ((uint64_t)i * (i + 3) - 4) * i / 6;
        P[i] = (seq_t)pk;
        B[i] = (seq_t)1;     /* C(p(k),0) = 1 */
        A[i] = (seq_t)0;     /* will be set when it is computed */
    }
    CAP = new_cap;
}

/* Compute a(n) and any missing values up to n */
static void ensure_cache(idx_t n)
{
    if (COMPUTED > n)
        return;

    grow_capacity(n);

    if (COMPUTED == 0) {          /* initialise a(0) */
        A[0] = (seq_t)1;
        COMPUTED = 1;
    }

    for (idx_t cur = COMPUTED; cur <= n; ++cur) {
        /* update binomial values B[k] for k < cur to m = cur - k */
        for (idx_t k = 0; k < cur; ++k) {
            seq_t pk = P[k];
            idx_t m = cur - k;          /* new exponent */
            if (m == 1) {              /* previous m-1 == 0 */
                B[k] = pk;
                continue;
            }

            idx_t prev = m - 1;         /* previous exponent */
            if (prev > (idx_t)pk) {
                B[k] = (seq_t)0;
                continue;
            }

            seq_t numerator   = pk - (seq_t)prev;
            seq_t denominator = (seq_t)m;
            B[k] = (B[k] * numerator) / denominator;
        }

        /* compute a[cur] from the recurrence */
        seq_t posSum = (seq_t)0;
        seq_t negSum = (seq_t)0;
        for (idx_t k = 0; k < cur; ++k) {
            seq_t term = A[k] * B[k];
            if (((cur - k) & 1) == 0) posSum += term;
            else                      negSum += term;
        }

        seq_t result;
        if (negSum >= posSum)
            result = (seq_t)1 + (negSum - posSum);
        else
            result = (seq_t)1 - (posSum - negSum);

        A[cur] = result;
    }

    COMPUTED = n + 1;
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return A[n];
}
