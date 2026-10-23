/*
 * A178669  The number of permutations of [n] with 2 cycles of length 2.
 * https://oeis.org/A178669
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of permutations of [n] with exactly two cycles of
 *            length 2. Computation: a(n) = 3 * C(n,4) * B(n‑4) where B(m) is
 *            the number of permutations of m elements with no 2‑cycles, and
 *            B(m) = m! * Σ_{i=0}^{⌊m/2⌋} (-1)^i / (i! · 2^i)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 4) return 0;                       /* a(3) = 0, below offset irrelevant */

    /* Compute binomial coefficient C(n,4) */
    seq_t bin = 1;
    for (idx_t i = 1; i <= 4; ++i) {
        bin = bin * (n - i + 1) / i;
    }

    /* Let m = n – 4 */
    idx_t m = n - 4;

    /* Compute m! and the alternating sum for B(m) */
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i) fact *= i;

    seq_t sum = fact;            /* term for i = 0 */
    seq_t term = fact;           /* current term */

    for (idx_t i = 1; i <= m / 2; ++i) {
        term = term / (i * 2);   /* term_i = term_{i‑1} / (i·2)  */
        if (i & 1) {             /* odd i → subtract */
            sum -= term;
        } else {                 /* even i → add */
            sum += term;
        }
    }

    /* Combine to obtain a(n) */
    seq_t result = bin;
    result *= (seq_t)3;
    result *= sum;
    return result;
}
