/*
 * A177010  G.f.: ((1-q)^2+(1+q)*sqrt(1-6*q+q^2))/2.
 * https://oeis.org/A177010
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 14, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A177010
 *  G.f. = ((1-q)^2 + (1+q)*sqrt(1-6*q+q^2))/2
 *
 *  a(n) is the coefficient of q^n in G(q).  The terms are
 *  a(0)=1, a(1)=-2, a(2)=-3, a(3)=-8, …
 *
 *  Computation:
 *      1. Compute the coefficients S_k of the series
 *         S(q) = sqrt(1-6q+q^2)   using the recurrence
 *             S_0 = 1,  S_1 = -3,  S_2 = -4,
 *             S_k = -( 1/2 ) * Σ_{i=1}^{k-1} S_i * S_{k-i}   for k ≥ 3.
 *
 *      2. Let R_k = S_k + S_{k-1}  (with S_{-1}=0).  Then
 *         R(q) = (1+q)*S(q).
 *
 *      3. a(n) = ( (1-q)^2 + R(q) )_n  / 2:
 *         if n==0   a(0) = (1 + R_0) / 2
 *         if n==1   a(1) = (‑2 + R_1) / 2
 *         if n==2   a(2) = (1  + R_2) / 2
 *         otherwise a(n) = R_n / 2
 *
 *  Memoisation of the S sequence allows calls in any order.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Edge case: offset 0 */
    if (n == 0)
        return (seq_t)1;

    /* Static cache for the S sequence */
    static seq_t *S   = NULL;
    static size_t S_cap = 0;       /* current allocation size   */
    static size_t S_len = 0;       /* number of elements stored */

    /* One‑time initialisation */
    if (S == NULL) {
        S_cap = 8;
        S     = seq_alloc(S_cap);
        if (S == NULL) return (seq_t)0;   /* allocation failure – unlikely       */
        S[0] = (seq_t)1;      /* S_0   */
        S[1] = (seq_t)-3;     /* S_1   */
        S[2] = (seq_t)-4;     /* S_2   */
        S_len = 3;
    }

    /* Extend the cache up to index n */
    while (S_len <= (size_t)n) {
        idx_t k = (idx_t)S_len;      /* compute S_k */
        seq_t sum = 0;
        for (idx_t i = 1; i < k; ++i) {
            sum += S[i] * S[k - i];
        }
        seq_t Sk = -sum / (seq_t)2;   /* recurrence for k ≥ 3   */

        /* Grow the storage if needed */
        if (S_len == S_cap) {
            size_t new_cap = S_cap * 2;
            seq_t *tmp = seq_realloc(S, S_cap, new_cap);
            if (tmp == NULL) return (seq_t)0;   /* allocation failure   */
            S = tmp;
            S_cap = new_cap;
        }

        S[S_len] = Sk;
        ++S_len;
    }

    /* R_n = S_n + S_{n-1} (with S_{-1}=0) */
    seq_t R_n = S[(size_t)n];
    if (n > 0)
        R_n += S[(size_t)n - 1];

    /* Compute the numerator t_n of a(n) */
    seq_t t;
    if (n == 1)
        t = -(seq_t)2 + R_n;
    else if (n == 2)
        t = (seq_t)1 + R_n;
    else
        t = R_n;

    /* a(n) = t / 2  */
    return t / (seq_t)2;
}
