/*
 * A054666  Number of 6-ary Lyndon words with trace 1 mod 6.
 * https://oeis.org/A054666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of primitive necklaces of length n over the
 *            6‑symbol alphabet {0,…,5} whose digit sum ≡ 1 (mod 6). Computed
 *            via the recurrence f[n][r] = 6^(n-1) - Σ_{d|n, d<n} Σ_{t}
 *            f[d][t] * [ (n/d)*t ≡ r (mod 6) ], where f[n][r] is the number
 *            of *periodic* length‑n words with trace ≡ r (mod 6). T...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The build supplies the integer types seq_t and idx_t. */
seq_t a(idx_t n)
{
    /* Static state: pre‑computed powers of 6 and the table f[n][r]. */
    static size_t maxN = 0;
    static seq_t *pow6 = NULL;          /* pow6[i] = 6^i */
    static seq_t *f     = NULL;          /* f[n*6 + r] entry */

    /* Expand tables if the requested n is larger than already computed. */
    if (n > maxN) {
        /* Resize pow6 */
        seq_t *new_pow6 = (seq_t *)realloc(pow6, (n + 1) * sizeof(seq_t));
        if (!new_pow6) exit(1);          /* allocation failure */
        pow6 = new_pow6;
        if (maxN == 0) pow6[0] = (seq_t)1;
        for (idx_t i = maxN + 1; i <= n; ++i)
            pow6[i] = pow6[i - 1] * (seq_t)6;

        /* Resize f */
        seq_t *new_f = (seq_t *)realloc(f, (n + 1) * 6 * sizeof(seq_t));
        if (!new_f) exit(1);
        f = new_f;

        /* Compute entries for new indices */
        for (idx_t i = maxN + 1; i <= n; ++i) {
            /* Initialise all residues with the total number of words of length i. */
            for (int r = 0; r < 6; ++r)
                f[i * 6 + r] = pow6[i - 1];

            /* Subtract contributions from shorter periods d | i, d < i. */
            for (idx_t d = 1; d * d <= i; ++d) {
                if (i % d != 0) continue;

                /* First divisor d */
                if (d < i) {
                    idx_t factor = i / d;        /* integer multiplier */
                    for (int t = 0; t < 6; ++t) {
                        int prod = (int)((factor * (idx_t)t) % 6);
                        f[i * 6 + prod] -= f[d * 6 + t];
                    }
                }

                /* Second divisor i/d, distinct from d */
                idx_t e = i / d;
                if (e != d && e < i) {
                    idx_t factor = i / e;        /* which is d */
                    for (int t = 0; t < 6; ++t) {
                        int prod = (int)((factor * (idx_t)t) % 6);
                        f[i * 6 + prod] -= f[e * 6 + t];
                    }
                }
            }
        }
        maxN = n;
    }

    /* Return the desired term: f[n][1] / n. */
    return f[n * 6 + 1] / (seq_t)n;
}
