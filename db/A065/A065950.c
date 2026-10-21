/*
 * A065950  Bessel polynomial {y_n}'''(1).
 * https://oeis.org/A065950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the third derivative at 1 of the Bessel polynomial yₙ(x)
 *            * (OEIS A065950). An explicit combinatorial form is * * a(n) =
 *            ∑ₖ₌₃ⁿ (n+k)! / [(k‑3)! (n‑k)!] × 2⁻ᵏ * * By changing variables j
 *            = k – 3 this sum can be computed iteratively * in O(n) time with
 *            only integer arithmetic: * * let T₀ = (...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 3)            /* first three terms are zero */
        return 0;

    /* compute T0 = (n‑2)(n‑1)n(n+1)(n+2)(n+3) / 8  */
    seq_t T = 1;
    for (idx_t t = n - 2; t <= n + 3; ++t)
        T *= (seq_t)t;                /* product of six consecutive integers */
    T /= (seq_t)8;                    /* division is exact */

    seq_t sum = T;                    /* S = T0 */

    /* iterate j = 0 … n‑4 to generate T1 … T_{n‑3} */
    for (idx_t j = 0; j < n - 3; ++j) {
        seq_t num = (seq_t)(n + j + 4) * (seq_t)(n - j - 3); /* (n+j+4)(n‑j‑3) */
        seq_t den = (seq_t)(2 * (j + 1));                    /* 2(j+1) */
        T = T * num / den;                                 /* exact division */
        sum += T;
    }

    return sum;  /* a(n) = S */
}
