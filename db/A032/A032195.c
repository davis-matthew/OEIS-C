/*
 * A032195  Number of necklaces with 10 black beads and n-10 white beads.
 * https://oeis.org/A032195
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/n) * Σ_{d | gcd(n,10)} φ(d) * C(n/d, 10/d)
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 37, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where φ(1)=1, φ(2)=1, φ(5)=4, φ(10)=4. */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n < 10) return 0;        /* behaviour below offset does not matter */

    /* divisors of 10 that we need to consider */
    static const unsigned ds[4]   = {1, 2, 5, 10};
    static const unsigned phivals[4] = {1, 1, 4, 4};

    seq_t sum = 0;
    for (unsigned i = 0; i < 4; ++i) {
        unsigned d = ds[i];
        if (n % d != 0) continue;      /* d must divide n */
        idx_t nd = n / d;               /* n/d   */
        idx_t kd = 10 / d;              /* 10/d  */
        seq_t term = binom(nd, kd);     /* C(n/d,10/d) */
        term *= (seq_t)phivals[i];      /* multiply by φ(d) */
        sum += term;
    }
    return sum / (seq_t)n;
}
