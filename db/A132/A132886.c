/*
 * A132886  Triangle read by rows: T(n,k) is the number of paths in the right
 *          half-plane, from (0,0) to (n,0), consisting of steps U=(1,1),
 *          D=(1,-1), h=(1,0) and H=(2,0), having k U steps (0 <= k <=
 *          floor(n/2)).
 * https://oeis.org/A132886
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A132886 : Number of paths in the right half‑plane.
   We compute the triangle element T(r,k) by the sum
     T(r,k) = Σ_{t=0}^{⌊(r‑2k)/2⌋} C(r‑t, k) · C(r‑t‑k, k) · C(r‑t‑2k, t)
   The flattened index n is mapped to (r,k) by
     r = smallest integer with  Σ_{i=0}^{r‑1} (1+⌊i/2⌋) > n
     k = n − Σ_{i=0}^{r‑1} (1+⌊i/2⌋)          */

/* We provide an independent binomial implementation suitable for any
   seq_t that supports the operators + - * / % and comparisons. */

#include <stddef.h>
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* Helper: binomial coefficient.  Safe for any seq_t with the required
   operations.  Uses a multiplicative formula to keep the intermediate
   size low. */
static inline seq_t binom(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply first, then divide – the division is exact */
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

/* ------------------------------------------------------------------ */
/* Compute T(r,k) using the closed‑form sum. */
static inline seq_t triangle_value(idx_t r, idx_t k) {
    if (k > (r + 1) / 2) return 0;            /* since t>=0 cannot satisfy 2t+2k<=r */
    idx_t max_t = (r - 2 * k) / 2;
    seq_t total = 0;
    for (idx_t t = 0; t <= max_t; ++t) {
        seq_t a = binom(r - t, k);
        seq_t b = binom(r - t - k, k);
        seq_t c = binom(r - t - 2 * k, t);
        seq_t term = a * b;
        term = term * c;
        total += term;
    }
    return total;
}

/* ------------------------------------------------------------------ */
/* Return the nth term of the sequence (0‑based). */
seq_t a(idx_t n) {
    /* Find the row r such that cumulative elements of previous rows
       are ≤ n < cumulative elements up to row r. */
    idx_t cumulative = 0;
    idx_t r = 0;
    while (true) {
        idx_t len = 1 + (r / 2);              /* number of terms in row r */
        if (cumulative + len > n) break;      /* n lies in this row */
        cumulative += len;
        ++r;
    }
    idx_t k = n - cumulative;                  /* column inside row r */
    return triangle_value(r, k);
}
