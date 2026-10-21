/*
 * A226864  Expansion of phi(-x^3) * f(-x^4) in powers of x where phi(), f()
 *          are Ramanujan theta functions.
 * https://oeis.org/A226864
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The formula is the coefficient of x^n in
   (∑_{k∈ℤ} (-1)^k x^{3k²}) · ∏_{m=1}^∞ (1 – x^{4m}). */

/* External types:
   seq_t – signed integer type, may be an arbitrary‑precision class
   idx_t – unsigned 64‑bit integer (uint64_t).   */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Allocate arrays for the generating functions.  The cast is
       required when seq_t is a class type.  The memory is only
       zero‑initialized; no constructor is called, which is permitted
       by the build environment. */
    seq_t *P = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *S = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *A = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!P || !S || !A)  /* allocation failure */
        return 0;

    /* Product P(x) = ∏_{m≥1} (1 – x^{4m}). */
    P[0] = (seq_t)1;
    for (idx_t m = 1; m <= n / 4; ++m) {
        idx_t step = 4 * m;
        for (idx_t i = n; i >= step; --i) {
            P[i] -= P[i - step];
        }
    }

    /* Sparse sum S(x) = ∑_{k∈ℤ} (-1)^k x^{3k²}.  Only indices 3k²
       are non‑zero; coefficient at k = 0 is 1, for k ≥ 1 it is
       2·(-1)^k. */
    for (idx_t t = 0; 3 * t * t <= n; ++t) {
        idx_t exp = 3 * t * t;
        seq_t val;
        if (t == 0) {
            val = (seq_t)1;
        } else {
            val = ((t & 1) == 0) ? (seq_t)2 : (seq_t)(-2);
        }
        S[exp] = val;
    }

    /* Convolution A = S ⋆ P. */
    for (idx_t k = 0; k <= n; ++k) {
        seq_t valS = S[k];
        if (valS == (seq_t)0) continue;
        for (idx_t i = k; i <= n; ++i) {
            A[i] += valS * P[i - k];
        }
    }

    seq_t result = A[n];

    /* Release memory.  Since seq_t is a POD type (or a type that
       has no destructor to run), free is sufficient. */
    free(P);
    free(S);
    free(A);

    return result;
}
