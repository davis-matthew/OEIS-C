/*
 * A017903  Expansion of 1/(1 - x^9 - x^10 - ...).
 * https://oeis.org/A017903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(n)=0 for 1≤n<9; for n≥9 a(n) = Σ_{k=0}^{n-9} a(k).
 *            This is the number of compositions of n with parts ≥9,
 *            equivalently the number of binary words of length n−9 with 0
 *            appearing only in runs whose length is a multiple of 9
 * Range:     u8 n <= 42, u16 n <= 70, u32 n <= 128, u64 n <= 243,
 *            u128 n <= 472, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state: arrays of a(n) and their prefix sums */
    static seq_t *A = NULL;           /* values a(i)      */
    static seq_t *P = NULL;           /* prefix sums P(i)=Σ_{k=0}^{i} a(k) */
    static idx_t capacity = 0;        /* allocated size   */
    static idx_t computed = 0;        /* highest index computed   */

    /* --- initialise --- */
    if (A == NULL) {
        capacity = 1;
        A = (seq_t *)malloc(capacity * sizeof(seq_t));
        P = (seq_t *)malloc(capacity * sizeof(seq_t));
        /* a(0)=1 ; prefix P(0)=1 */
        A[0] = (seq_t)1;
        P[0] = (seq_t)1;
        computed = 0;
    }

    /* --- extend allocation if needed --- */
    if (n + 1 > capacity) {
        /* grow capacity to at least n+1, doubling each time */
        while (capacity < n + 1) capacity *= 2;
        A = (seq_t *)realloc(A, capacity * sizeof(seq_t));
        P = (seq_t *)realloc(P, capacity * sizeof(seq_t));
    }

    /* --- compute values up to n --- */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            if (i < 9) {
                A[i] = (seq_t)0;
            } else {
                /* a(i) = P[i-9] = sum_{k=0}^{i-9} a(k) */
                A[i] = P[i - 9];
            }
            P[i] = P[i - 1] + A[i];
        }
        computed = n;
    }

    return A[n];
}
