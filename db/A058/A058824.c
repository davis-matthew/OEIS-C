/*
 * A058824  a(0) = 1, a(1) = 9; for n >= 2 a(n) is the number of degree-n
 *          monic reducible polynomials over GF(9), i.e., a(n) = 9^n -
 *          A027381(n).
 * https://oeis.org/A058824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 9; for n >= 2 a(n) = 9^n - (1/n) * Σ_{d|n} μ(d)
 *            * 9^{n/d} (number of degree‑n monic reducible polynomials over
 *            GF(9) ) where μ is the Möbius function
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/big_pow.h"
#include "../utils/mu_of.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)9;

    /* total monic polynomials: 9^n */
    seq_t pow9 = big_pow((seq_t)9, (seq_t)n);

    /* Σ μ(d) 9^{n/d}  =  Σ_{d|n} μ(d) * 9^{n/d}  */
    seq_t sum_pos = 0;   /* μ(d) = +1 terms   */
    seq_t sum_neg = 0;   /* μ(d) = -1 terms   */

    for (idx_t d = 1; d <= n; ++d) {
        if (n % d != 0) continue;
        int mu = mu_of(d);
        seq_t term = big_pow((seq_t)9, (seq_t)(n / d));
        if (mu == 1)      sum_pos += term;
        else if (mu == -1) sum_neg += term;
        /* μ(d) = 0 contributes nothing */
    }

    /* n * (number of monic irreducibles) = sum_pos - sum_neg */
    seq_t diff = sum_pos;
    diff -= sum_neg;
    seq_t irred = diff / (seq_t)n;

    /* reducible count */
    return pow9 - irred;
}
