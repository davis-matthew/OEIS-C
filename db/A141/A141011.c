/*
 * A141011  E.g.f. exp(sum_{d|M} (exp(d*x)-1)/d), M=15.
 * https://oeis.org/A141011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} C(n‑1, k‑1) * S_k * a(n‑k), where S_k = 1 +
 *            3^{k‑1} + 5^{k‑1} + 15^{k‑1} and a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   seq_alloc_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>                 /* size_t for memory allocation */
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"

/* External types supplied by the build */
typedef uint64_t idx_t;            /* index type */

/* Static storage for the sequence values and the S_k values */
static seq_t *A               = NULL; /* a(n) array, indices 0..A_high */
static idx_t  A_high          = 0;
static idx_t  A_cap           = 0;

static seq_t *S               = NULL; /* S_k array, indices 1..S_high */
static idx_t  S_high          = 0;
static idx_t  S_cap           = 0;

/* Current powers for S_k computation:
   powX holds X^{current_k-1} before computing S[current_k]. */
static seq_t pow3 = (seq_t)1;     /* 3^{k-1} */
static seq_t pow5 = (seq_t)1;     /* 5^{k-1} */
static seq_t pow15 = (seq_t)1;    /* 15^{k-1} */

/* Ensure the arrays A and S are large enough and compute any missing
   values up to index n (inclusive). */
static void ensure_avail(idx_t n)
{
    /* ---------- Allocate / resize A ---------- */
    if (A == NULL) {
        A_cap = 16;
        A = seq_alloc_array(A_cap + 1);
        A[0] = (seq_t)1;          /* a(0) = 1 */
        A_high = 0;
    }
    if (n > A_cap) {
        idx_t new_cap = A_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_array(new_cap + 1);
        for (idx_t i = 0; i <= A_cap; ++i) new_arr[i] = A[i];
        seq_free_array(A);
        A = new_arr;
        A_cap = new_cap;
    }

    /* ---------- Allocate / resize S ----------
       Note: S is 1-indexed, we allocate space for index up to n. */
    if (S == NULL) {
        S_cap = 16;
        S = seq_alloc_array(S_cap + 1);
        S_high = 0;               /* no S values computed yet */
    }
    if (n > S_cap) {
        idx_t new_cap = S_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_arr = seq_alloc_array(new_cap + 1);
        for (idx_t i = 0; i <= S_cap; ++i) new_arr[i] = S[i];
        seq_free_array(S);
        S = new_arr;
        S_cap = new_cap;
    }

    /* ---------- Compute missing S_k values ----------
       pow3, pow5, pow15 currently hold powers for current_k = S_high + 1. */
    for (idx_t k = S_high + 1; k <= n; ++k) {
        S[k] = (seq_t)1 + pow3 + pow5 + pow15;   /* S_k = 1 + 3^{k-1} + 5^{k-1} + 15^{k-1} */
        /* Update powers for the next k */
        pow3  *= (seq_t)3;
        pow5  *= (seq_t)5;
        pow15 *= (seq_t)15;
    }
    S_high = n;

    /* ---------- Compute missing a[n] values ----------
       Use the recurrence with binomial coefficients. */
    for (idx_t i = A_high + 1; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1;       /* binom(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = S[k];
            term *= binom;
            term *= A[i - k];
            sum += term;
            if (k < i) {
                /* binom(i-1, k) = binom(i-1, k-1) * (i - k) / k */
                seq_t mult = (seq_t)(i - k);
                binom = (binom * mult) / (seq_t)k;
            }
        }
        A[i] = sum;
    }
    A_high = n;
}

/* Public function implementing the sequence. */
seq_t a(idx_t n)
{
    ensure_avail(n);
    return A[n];
}
