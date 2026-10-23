/*
 * A186196  a(n)=(-1)^n*(-2)^C(n,2)*A001045(n+1).
 * https://oeis.org/A186196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-1)^n * (-2)^{C(n,2)} * A001045(n+1)
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 7, i64 n <= 10, i128 n <= 14,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* --- Precompute A001045 values up to n+1 --------------------- */
    static seq_t *A = NULL;          /* A[0] is unused, A[1] = 1, A[2] = 1 */
    static idx_t A_cap = 0;          /* current allocated size (indices 0 .. A_cap-1) */

    idx_t need = n + 1;              /* maximum index required in A[] */

    /* Initial allocation */
    if (A == NULL) {
        A_cap = 3;                   /* indices 0,1,2 */
        A = (seq_t*)malloc(A_cap * sizeof(seq_t));
        A[0] = (seq_t)0;
        A[1] = (seq_t)1;
        A[2] = (seq_t)1;
    }

    /* Extend the array if necessary */
    if (need >= A_cap) {
        A = (seq_t*)realloc(A, (need + 1) * sizeof(seq_t));
        for (idx_t k = A_cap; k <= need; ++k) {
            seq_t term = A[k - 1] + A[k - 1];   /* 2 * A[k-1] */
            if (k & 1)                         /* odd k : +1 */
                term += (seq_t)1;
            else                               /* even k : -1 */
                term -= (seq_t)1;
            A[k] = term;
        }
        A_cap = need + 1;
    }

    /* --- Compute the main term --------------------------------- */
    seq_t part_val   = A[need];                      /* A001045(n+1)   */
    idx_t  k_idx     = (idx_t)n * ((idx_t)n - 1) / 2;/* C(n,2) as idx_t */
    seq_t  pow2_k    = big_pow((seq_t)2, (seq_t)k_idx);/* 2^{C(n,2)} */

    bool neg = ((n & 1) ^ (k_idx & 1));           /* parity of n + C(n,2) */
    seq_t  result = pow2_k * part_val;

    if (neg)
        result = -result;

    return result;
}
