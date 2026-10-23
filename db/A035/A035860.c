/*
 * A035860  Coordination sequence for A_34 lattice.
 * https://oeis.org/A035860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   A035860 * a(0) = 1 * a(n) = Σ_{d=1..34} C(35,d) * C(n-1,d-1) *
 *            C(34-d+n,n) for n ≥ 1 * (C(x,y) is the binomial coefficient; all
 *            arguments are non‑negative integers)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"


/* Small binomial coefficient with k ≤ 33 (works for both seq_t and
 * arbitrary‑precision seq_t by performing gcd cancellations). */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;     /* symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        seq_t num = (seq_t)(n - k + i);
        seq_t den = (seq_t)i;

        seq_t g1 = gcd(res, den);
        res /= g1;
        den /= g1;

        seq_t g2 = gcd(num, den);
        num /= g2;
        den /= g2;

        res *= num;
        if (den != 1)
            res /= den;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;           /* a(0) = 1 */

    /* Pre‑compute C(35,d) once. */
    static seq_t comb35[36];
    static bool init = false;
    if (!init)
    {
        for (idx_t d = 0; d <= 35; ++d)
            comb35[d] = binom_small(35, d);
        init = true;
    }

    seq_t total = 0;
    /* In the sum only d up to min(34,n) can contribute. */
    idx_t max_d = n < 34 ? n : 34;
    for (idx_t d = 1; d <= max_d; ++d)
    {
        seq_t c1 = comb35[d];                                 /* C(35,d)        */
        seq_t c2 = binom_small(n - 1, d - 1);                 /* C(n-1,d-1)     */
        seq_t c3 = binom_small(n + 34 - d, 34 - d);           /* C(n+34-d,34-d) */

        seq_t term = c1 * c2 * c3;
        total += term;
    }
    return total;
}
