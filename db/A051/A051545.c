/*
 * A051545  Second unsigned column of triangle A051339.
 * https://oeis.org/A051545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * Σ_{k=0}^{n-1} (binom(6+k,k) / (n-k)), * with a(0) =
 *            0. * Implementation uses big‑integer arithmetic (seq_t) with
 *            only * +, -, *, / on small integers, satisfying the build
 *            constraints
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(0) = 0 */

    /* Compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i) {
        fact *= (seq_t)i;
    }

    /* term_factor = fact / n  (exact) */
    seq_t term_factor = fact / (seq_t)n;

    seq_t binom = 1;    /* binom(6+0,0) */
    seq_t sum = 0;

    for (idx_t k = 0; k < n; ++k) {
        /* add current term */
        sum += term_factor * binom;

        if (k == n - 1) break;    /* last term reached */

        /* update term_factor for next k:
           term_factor_{k+1} = term_factor_k * (n-k) / (n-k-1) */
        term_factor = (term_factor * (seq_t)(n - k)) /
                      (seq_t)(n - k - 1);

        /* update binom for next k:
           binom_{k+1} = binom_k * (6+k+1) / (k+1) */
        binom = (binom * (seq_t)(6 + k + 1)) / (seq_t)(k + 1);
    }

    return sum;
}
