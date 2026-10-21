/*
 * A225601  a(n) = A000172(n)^n, where A000172(n) = Sum_{k=0..n}
 *          binomial(n,k)^3 forms the Franel numbers.
 * https://oeis.org/A225601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (Σ_{k=0..n} binomial(n,k)^3)^n, where binomial(n,k) =
 *            C(n,k)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

/* Return a(n) for n >= 0 */
seq_t a(idx_t n)
{
    /* Compute Franel number F(n) = Σ_{k=0..n} C(n,k)^3 */
    seq_t C   = 1;   /* C(n,0) */
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        if (k > 0)
        {
            /* C(n,k) = C(n,k-1) * (n - k + 1) / k */
            C = C * (seq_t)(n - k + 1) / (seq_t)k;
        }

        seq_t C3 = C * C * C;
        sum += C3;
    }

    /* Return F(n)^n */
    return seq_pow(sum, n);
}
