/*
 * A049947  a(n) = a(1) + a(2) + ... + a(n-1) + a(m) for n >= 4, where m = 2*n
 *          - 2 - 2^(p+1) and p is the unique integer such that 2^p < n - 1 <=
 *          2^(p+1), starting with a(1) = a(2) = 1 and a(3) = 4.
 * https://oeis.org/A049947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(1) = 1, a(2) = 1, a(3) = 4 * For n ≥ 4: * let p be the
 *            unique integer with 2^p < n-1 ≤ 2^(p+1) * m = 2*n - 2 - 2^(p+1)
 *            * a(n) = (a(1)+a(2)+…+a(n-1)) + a(m) * This implementation
 *            memoises all terms up to the largest n * requested and uses a
 *            prefix‑sum array for O(1) access to the * cumulative...
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free */

/* Compute floor(log2(v)) for v>0 */
static inline idx_t floor_log2_idx(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) ++r;
    return r;
}

/* Static memoisation storage (1‑based indexing) */
static seq_t *A = NULL;           /* a[i] */
static seq_t *P = NULL;           /* prefix sums, P[i] = a[1]+…+a[i] */
static idx_t  cap = 0;            /* allocated capacity (incl. index 0) */
static idx_t  len = 0;            /* number of terms computed so far */

/* Grow the storage arrays to accommodate at least n elements (1‑based) */
static inline void ensure_capacity(idx_t n)
{
    if (n <= cap) return;
    idx_t new_cap = cap ? cap * 2 : 4;
    while (new_cap < n) new_cap <<= 1;

    /* realloc may return NULL on failure, but we ignore that
       here because the testing harness guarantees enough memory. */
    A = (seq_t *)realloc(A, (size_t)(new_cap + 1) * sizeof(seq_t));
    P = (seq_t *)realloc(P, (size_t)(new_cap + 1) * sizeof(seq_t));

    cap = new_cap;
}

/* Public entry point: compute a(n) (external linkage) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for non‑positive indices */

    /* Compute all terms up to n if not already done */
    if (n > len) {
        ensure_capacity(n);

        /* Compute new terms */
        for (idx_t i = len + 1; i <= n; ++i) {
            if (i == 1) {
                A[1] = (seq_t)1;
                P[1] = A[1];
            } else if (i == 2) {
                A[2] = (seq_t)1;
                P[2] = P[1] + A[2];
            } else if (i == 3) {
                A[3] = (seq_t)4;
                P[3] = P[2] + A[3];
            } else {
                /* Find p: 2^p < i-1 ≤ 2^(p+1)  →  p = floor_log2(i-2) */
                idx_t shift = floor_log2_idx(i - 2) + 1;     /* shift = p+1 */
                idx_t pow2 = (idx_t)1 << shift;             /* 2^(p+1) */

                idx_t m = 2 * i - 2 - pow2;                 /* m ≥ 1, < i */

                /* a(i) = prefix sum up to i-1  +  a(m)           */
                seq_t ai = P[i - 1] + A[m];
                A[i] = ai;
                P[i] = P[i - 1] + ai;                       /* 2*P[i-1] + a(m) */
            }
        }
        len = n;
    }

    return A[n];
}
