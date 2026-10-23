/*
 * A008362  Crystal ball sequence for D_8 lattice.
 * https://oeis.org/A008362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            (1+104*x+1724*x^2+7768*x^3+12550*x^4+7768*x^5+1724*x^6+104*x^7+x^8)/(1-x)^9
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 15, u64 n <= 263, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute binomial coefficient C(n,k) for small k (here k <= 8). */
static seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;          /* use symmetry      */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i);
        res = res / (seq_t)i;
    }
    return res;
}

/* Crystal ball sequence for D8 lattice. */
seq_t a(idx_t n)
{
    static const idx_t coef[9] = {1, 104, 1724, 7768, 12550, 7768, 1724, 104, 1};
    seq_t sum = 0;
    for (idx_t i = 0; i <= 8 && i <= n; ++i) {
        idx_t m = n - i + 8;            /* m >= 8 for all i <= n */
        seq_t c = binom(m, 8);          /* C(m,8) */
        sum += (seq_t)coef[i] * c;
    }
    return sum;
}
