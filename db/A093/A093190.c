/*
 * A093190  Array t read by antidiagonals: number of {112,212}-avoiding words.
 * https://oeis.org/A093190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) is obtained by reading the array t(k,ℓ) (k = alphabet
 *            size, * ℓ = word length) along its antidiagonals. * * The array
 *            entry is * * t(k,ℓ) = ∑_{j=1}^{min(k,ℓ)} j! · C(k, j) · C(ℓ − 1,
 *            j − 1) * * where C(·,·) is the binomial coefficient. * * The
 *            index n of the sequence satisfies * a(1) = t(1...
 * Range:     u8 n <= 31, u16 n <= 84, u32 n <= 240, u64 n <= 682,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* safety for n = 0; offset starts at 1 */

    /* Find the diagonal s = k + ℓ, with 1‑based k, ℓ.  The
     * total number of terms up to diagonal s is (s‑1)·s/2. */
    idx_t s = 2;
    while ((s - 1) * s / 2 < n) ++s;

    /* Number of terms before the current diagonal. */
    idx_t prev = (s - 2) * (s - 1) / 2;

    /* Position within the diagonal (1‑based). */
    idx_t pos = n - prev;              /* 1 ≤ pos ≤ s‑1 */

    /* Alphabet size (k) and word length (ℓ). */
    idx_t k   = pos;
    idx_t len = s - pos;

    /* Compute t(k, len). */
    idx_t jmax = k <= len ? k : len;
    seq_t result = 0;
    seq_t jfact  = 1;                    /* will hold j! */

    for (idx_t j = 1; j <= jmax; ++j) {
        jfact *= (seq_t)j;                /* j! */

        seq_t b1 = binomial(k, j);            /* C(k, j)          */
        seq_t b2 = binomial(len - 1, j - 1);  /* C(len‑1, j‑1)    */

        seq_t term = jfact;
        term *= b1;
        term *= b2;

        result += term;
    }

    return result;
}
