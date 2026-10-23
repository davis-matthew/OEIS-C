/*
 * A103419  Number of compositions of n in which the least part is odd.
 * https://oeis.org/A103419
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1}^n (-1)^{k+1} Σ_{t=1}^{⌊n/k⌋} binom(n-(k-1)·t-1,
 *            t-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)                /* offset 1; behaviour for n<1 is irrelevant */
        return 0;

    /* compositions with all parts ≥1 : 2^(n-1) */
    seq_t total = pow2(n - 1);

    for (idx_t k = 2; k <= n; ++k) {
        idx_t max_t = n / k;
        seq_t sumk = 0;

        for (idx_t t = 1; t <= max_t; ++t) {
            idx_t N = n - (k - 1) * t - 1;
            idx_t K = t - 1;
            sumk += binom(N, K);
        }

        if ((k & 1) == 0)      /* even k → negative sign */
            total -= sumk;
        else                    /* odd k  → positive sign */
            total += sumk;
    }

    return total;
}
