/*
 * A116418  Expansion of q^(-1/3) * b(q) * c(q) * b(q^2) / 3 in powers of q
 *          where b(), c() are cubic AGM theta functions.
 * https://oeis.org/A116418
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 109, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A116418: coefficients of the product  
   ∏_{k≥1} (1 - x^k)^2 (1 - x^{2k})^3 (1 - x^{3k})^2 / (1 - x^{6k}) .
   The recurrence used is  
     a(0) = 1
     a(n) = -(1/n) * Σ_{k=1}^{n} s(k) * a(n-k)   (n ≥ 1)  
   where
     e(d) = 2 + 3·[d even] + 2·[d ≡ 0 (mod 3)] - [d ≡ 0 (mod 6)]
     s(k) = Σ_{d∣k} d · e(d)                      .
   The implementation keeps a cached coefficient array, grows it
   as needed, and recomputes the s‑array from scratch whenever the
   capacity increases.  All operations are integer arithmetic.  */

#include <stdlib.h>

static seq_t *A = NULL;   /* coefficient cache, A[i] = a(i) */
static seq_t *S = NULL;   /* divisor‑sum array, S[k] = Σ_{d|k} d·e(d) */
static size_t cap = 0;    /* current allocated capacity for arrays */

seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;        /* required capacity */
    if (need > cap) {                   /* grow arrays and recompute */
        size_t new_cap = need < 2 ? 2 : need * 2;
        if (new_cap < need) new_cap = need;
        A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        S = (seq_t *)realloc(S, new_cap * sizeof(seq_t));

        /* recompute the divisor‑sum array S[1..new_cap-1] */
        for (size_t i = 0; i < new_cap; ++i) S[i] = (seq_t)0;
        for (size_t d = 1; d < new_cap; ++d) {
            int e = 2;                                   /* base exponent */
            if ((d & 1) == 0) e += 3;                    /* 2·k terms */
            if (d % 3 == 0)   e += 2;                    /* 3·k terms */
            if (d % 6 == 0)   e -= 1;                    /* divide by 6·k term */
            seq_t ed = (seq_t)d * (seq_t)e;              /* d·e(d) */
            for (size_t m = d; m < new_cap; m += d) {
                S[m] += ed;
            }
        }

        /* compute coefficients a[0..new_cap-1] */
        A[0] = (seq_t)1;
        for (size_t idx = 1; idx < new_cap; ++idx) {
            seq_t sum = (seq_t)0;
            for (size_t k = 1; k <= idx; ++k) {
                sum += S[k] * A[idx - k];
            }
            A[idx] = -(sum / (seq_t)idx);
        }
        cap = new_cap;
    }
    return A[(size_t)n];
}
