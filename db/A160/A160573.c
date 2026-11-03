/*
 * A160573  G.f.: Product_{k >= 0} (1 + x^(2^k-1) + x^(2^k)).
 * https://oeis.org/A160573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{i≥0} binomial(popcount(n + i), i) (A160573) The sum is
 *            finite because binomial(m,i)=0 for i>m
 * Range:     u8 n <= 79, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t binomial_pop(unsigned int m, unsigned int r)
{
    /* Compute binomial(m,r) for 0 ≤ r ≤ m using multiplicative formula.
       The value fits in seq_t for the given problem constraints.        */
    if (r > m) return 0;
    if (r == 0 || r == m) return 1;
    if (r > m - r) r = m - r;        /* use symmetry */
    seq_t res = 1;
    for (unsigned int k = 1; k <= r; ++k) {
        res = (res * (seq_t)(m - r + k)) / (seq_t)k;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; ; ++i) {
        idx_t val = n + i;
        unsigned int pop = __builtin_popcountll(val);
        if (i > pop) break;
        sum += binomial_pop(pop, (unsigned int)i);
    }
    return sum;
}
