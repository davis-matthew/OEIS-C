/*
 * A094303  a(1) = 1, a(2) = 2, and a(n+1) = a(n) * sum of all previous terms
 *          up to a(n-1) for n >= 2.
 * https://oeis.org/A094303
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1, a(2) = 2; for n ≥ 2, a(n+1) = a(n) *
 *            (a(1)+a(2)+…+a(n-1))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 10, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed terms and their cumulative sums */
    static seq_t *A = NULL;          /* a values (1‑based) */
    static seq_t *S = NULL;          /* prefix sums: S[i] = sum_{k=1}^i a(k) */
    static idx_t cap = 0;            /* allocated capacity (≥ indexed size) */
    static idx_t top = 0;            /* highest n that has been computed */

    /* ensure at least n+1 elements (index 0 unused, S[0]=0) */
    if ((size_t)n + 1 > (size_t)cap) {
        size_t new_cap = cap ? cap << 1 : 8;
        while ((size_t)new_cap < (size_t)n + 1) new_cap <<= 1;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));
        cap = (idx_t)new_cap;
    }

    /* initialise the first two terms if not yet done */
    if (top == 0) {
        S[0] = (seq_t)0;          /* sum of zero terms */
        A[1] = (seq_t)1;
        S[1] = A[1];              /* sum up to term 1 */
        A[2] = (seq_t)2;
        S[2] = S[1] + A[2];        /* sum up to term 2 */
        top = 2;
    }

    /* compute missing terms up to n */
    for (idx_t i = top + 1; i <= n; ++i) {
        if (i == 1) {
            A[1] = (seq_t)1;
            S[1] = A[1];
        } else if (i == 2) {
            A[2] = (seq_t)2;
            S[2] = S[1] + A[2];
        } else {
            /* a[i] = a[i-1] * sum_{k=1}^{i-2} a(k) = a[i-1] * S[i-2] */
            A[i] = A[i-1] * S[i-2];
            S[i] = S[i-1] + A[i];
        }
    }
    top = n;
    return A[n];
}
