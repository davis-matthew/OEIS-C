/*
 * A128314  Triangle, A000012 * A128313.
 * https://oeis.org/A128314
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 39, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   small_mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A128314
   Formula: a(n) = (-1)^(j-1) *
            Σ_{d=1}^{i} μ(d) *
            Σ_{m=⌈j/d⌉}^{⌊i/d⌋} (-1)^(m-1) * binom(m-1, j-1),
   where i is chosen so that i*(i+1)/2 ≥ n > (i-1)*i/2
   and j = n - (i-1)*i/2.
   The implementation uses a multiplicative binomial routine and a small
   Möbius function for 1≤d≤i.  Complexity: O(i²) with i≈√(2n),
   which gives well within the time limit for n up to many thousands. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/small_mobius.h"

/* Compute binomial coefficient binom(n,k) for 0 ≤ k ≤ n ≤ 200.
   Uses multiplicative formula to avoid intermediate overflow. */
static seq_t binom_idx(idx_t n, idx_t k) {
    if (k > n) return (seq_t)0;
    if (k == 0 || k == n) return (seq_t)1;
    if (k > n - k) k = n - k;
    seq_t res = (seq_t)1;
    for (idx_t t = 1; t <= k; ++t) {
        res = res * (seq_t)(n - k + t);
        res = res / (seq_t)t;
    }
    return res;
}

/* Return: nth term of OEIS A128314, 1‑based index. */
seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;          /* undefined, but return 0 */

    /* Determine triangle coordinates (i, j) with 1‑based indices. */
    idx_t i = 1;
    /* Find smallest i such that i*(i+1)/2 ≥ n. */
    while ((seq_t)i * (i + 1) / 2 < n) ++i;
    idx_t j = n - (i - 1) * i / 2;       /* column in that row, 1‑based */

    seq_t sum_total = (seq_t)0;

    for (idx_t d = 1; d <= i; ++d) {
        int mu = small_mobius(d);    /* -1, 0, or 1 */
        if (mu == 0) continue;
        seq_t mu_seq = (seq_t)mu;

        /* m ranges from ceil(j/d) to floor(i/d). */
        idx_t m_low  = (j + d - 1) / d; /* ceil(j/d) */
        idx_t m_high = i / d;          /* floor(i/d) */
        if (m_low > m_high) continue;

        seq_t sum_d = (seq_t)0;
        for (idx_t m = m_low; m <= m_high; ++m) {
            seq_t term = binom_idx(m - 1, j - 1);
            /* Apply (-1)^(m-1). */
            if ((m & 1ULL) == 0)   /* m even → sign – */
                term = -term;
            sum_d += term;
        }
        sum_total += mu_seq * sum_d;
    }

    /* Apply outer sign (-1)^(j-1). */
    if ((j & 1ULL) == 0)                /* j even → sign – */
        sum_total = -sum_total;

    return sum_total;
}
