/*
 * A009565  Expansion of e.g.f. sinh(exp(x)*x).
 * https://oeis.org/A009565
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{odd m ≤ n} C(n,m) * m^{n-m}, where C(n,m) is the
 *            binomial coefficient. This follows from the e.g.f.
 *            sinh(exp(x)*x)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 22, u128 n <= 38,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sum = 0;

    /* Start with the first odd m = 1.
       binom(n,1) = n. */
    seq_t C = (seq_t)n;   /* C = C(n, m) */
    idx_t m = 1;

    while (m <= n) {
        /* Add the term for this odd m. */
        seq_t power = seq_pow((seq_t)m, (idx_t)(n - m));
        sum += C * power;

        /* Prepare C for the next odd m = m+2.
           First compute C(n, m+1). */
        if (m + 2 > n)
            break;

        C = C * (seq_t)(n - m) / (seq_t)(m + 1);
        /* Now C = C(n, m+1).  Next compute C(n, m+2). */
        C = C * (seq_t)(n - (m + 1)) / (seq_t)(m + 2);

        /* Advance to the next odd index. */
        m += 2;
    }

    return sum;
}
