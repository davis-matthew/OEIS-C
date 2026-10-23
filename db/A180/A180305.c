/*
 * A180305  G.f.: 1/(1 + x*d/dx log(eta(x))), where eta(x) is Dedekind's
 *          eta(q) function without the q^(1/24) factor.
 * https://oeis.org/A180305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0}^{n-1} sigma(n-k) * a(k), a(0)=1 where sigma(m)
 *            = sum of positive divisors of m
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static caches for the sequence and for sigma values. */
    static seq_t *A = NULL;   /* a(0..N) */
    static idx_t A_len = 0;   /* number of elements stored in A   */
    static seq_t *S = NULL;   /* sigma(1..N) */
    static idx_t S_len = 0;   /* number of elements stored in S   */

    /* Handle the base case immediately. */
    if (n == 0) {
        /* Ensure A[0] exists. */
        if (A == NULL) {
            A = (seq_t*)calloc(1, sizeof(seq_t));
            if (!A) return 0;
            A[0] = 1;
            A_len = 1;
        }
        return A[0];
    }

    /* If we already computed the desired entry, return it. */
    if (n < A_len) return A[n];

    /* Remember old lengths before expanding. */
    idx_t old_A_len = A_len;
    idx_t old_S_len = S_len;

    /* Expand the cache for a(n). */
    idx_t new_len = n + 1;                /* we need indices 0..n inclusive */
    if (A_len == 0) {
        A = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (!A) return 0;
        A[0] = 1;                         /* a(0) = 1 */
        A_len = new_len;
    } else if (new_len > A_len) {
        A = (seq_t*)realloc(A, new_len * sizeof(seq_t));
        if (!A) return 0;
        /* Zero the newly added entries. */
        for (idx_t i = old_A_len; i < new_len; ++i)
            A[i] = 0;
        A_len = new_len;
    }

    /* Expand and recompute sigma values if needed. */
    if (new_len > S_len) {
        seq_t *oldS = S;
        S = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (!S) { S = oldS; return 0; }
        for (idx_t i = 1; i < new_len; ++i) {
            for (idx_t j = i; j < new_len; j += i)
                S[j] += (seq_t)i;
        }
        S_len = new_len;
    }

    /* Compute all new a(i) values that were not previously stored. */
    for (idx_t i = (old_A_len == 0 ? 1 : old_A_len); i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k < i; ++k) {
            sum += A[k] * S[i - k];
        }
        A[i] = sum;
    }

    return A[n];
}
