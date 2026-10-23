/*
 * A157029  A007318 * A157019.
 * https://oeis.org/A157029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{d=1..n} Σ_{q=1..⌊n/d⌋} binom(d+q-2, q-1) * binom(n-1,
 *            n-d*q) where binom(a,b) is the binomial coefficient C(a,b). This
 *            follows from the expansion of the generating function given in
 *            the OEIS entry
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binom_n_k, seq_alloc_array, seq_free_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_n_k.h"
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_free_array.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Pre‑compute the row of binomials (n-1 choose r) for r = 0 .. n-1 */
    seq_t *row = seq_alloc_array((size_t)n);   /* size n -> indices 0 .. n-1 */
    if (!row) return (seq_t)0;                /* allocation failure */

    if (n > 0) row[0] = seq_one();            /* C(n-1,0) = 1 */
    for (idx_t r = 1; r < n; ++r) {
        seq_t prev = row[r - 1];
        seq_t mult = (seq_t)(n - 1 - (r - 1)); /* multiply by (n-1-(r-1)) */
        row[r] = (prev * mult) / (seq_t)r;    /* exact division */
    }

    seq_t total = (seq_t)0;

    for (idx_t d = 1; d <= n; ++d) {
        idx_t max_q = n / d;                  /* ⌊n/d⌋ */
        for (idx_t q = 1; q <= max_q; ++q) {
            idx_t k1 = d + q - 2;              /* n + q - 2 in formula */
            idx_t k2 = q - 1;                  /* q - 1 in formula */
            seq_t term1 = binom_n_k(k1, k2);
            idx_t r = n - d * q;               /* n - d*q in formula */
            seq_t term2 = row[r];
            total += term1 * term2;
        }
    }

    seq_free_array(row);
    return total;
}
