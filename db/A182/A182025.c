/*
 * A182025  a(n) = 31*binomial(2*n,n-4) + Sum_{i=1..n-4}
 *          binomial(2*n,n-4-i)*(4+i).
 * https://oeis.org/A182025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 31·binomial(2 n, n‑4) + Σ₍i=1₎ⁿ⁻⁴ binomial(2 n,
 *            n‑4‑i)·(4+i)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4) return 0;

    /* k = n-4, m = 2*n */
    idx_t k = n - 4;
    idx_t m = n + n;            /* 2*n */

    /* Sum part: Σ_{j=0}^{k‑1} binomial(m, j) * (4 + k - j) */
    seq_t sum  = 0;
    seq_t bin  = 1;              /* binomial(m, 0) */

    for (idx_t j = 0; j < k; ++j) {
        /* term for this j */
        sum += bin * ((seq_t)(4 + k - j));

        /* next binomial value: C(m, j+1) = C(m, j) * (m-j)/(j+1) */
        if (j + 1 < k) {         /* avoid division by zero at final step */
            bin = bin * ((seq_t)(m - j)) / ((seq_t)(j + 1));
        }
    }

    /* First part: 31·C(m, k) */
    seq_t first = 31 * binomial(m, k);

    return first + sum;
}
