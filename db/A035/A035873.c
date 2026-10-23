/*
 * A035873  Coordination sequence for A_47 lattice.
 * https://oeis.org/A035873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula (OEIS A035873 – Coordination sequence for A_47 lattice):
 *
 * a(0) = 1
 * a(n) = Σ_{d=1}^{47} C(48,d) * C(n-1,d-1) * C(n+47-d, 47-d)   for n ≥ 1
 *
 * Here C(a,b) denotes the binomial coefficient “a choose b”.
 */

#include <stdint.h>

/* Helper: compute C(a,b) for 0 ≤ b ≤ 46 and a up to several thousand.
   The result is exact; division is guaranteed to be exact. */
static inline seq_t binom_small(idx_t a, idx_t b)
{
    if (b > a)
        return (seq_t)0;
    if (b > a - b)   /* use symmetry */
        b = a - b;
    seq_t res = (seq_t)1;
    for (idx_t i = 1; i <= b; ++i) {
        res *= (seq_t)(a - b + i);
        res /= (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* pre‑compute the small binomial table C(48,d) for d=0..47 */
    seq_t C48[48];
    for (idx_t d = 0; d <= 47; ++d)
        C48[d] = binom_small(48, d);

    /* the sum over d; stop when d > n because C(n-1,d-1)=0 beyond that */
    idx_t maxd = 47;
    if (n < maxd)    /* when n < 47 we can ignore larger d */
        maxd = n;

    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d <= maxd; ++d) {
        seq_t term = C48[d];                      /* C(48,d)   */
        term *= binom_small(n - 1, d - 1);        /* C(n-1,d-1) */
        term *= binom_small(n + 47 - d, 47 - d);  /* C(n+47-d,47-d) */
        sum += term;
    }
    return sum;
}
