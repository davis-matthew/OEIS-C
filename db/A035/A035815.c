/*
 * A035815  Coordination sequence for lattice D*_60 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(60,n)} 2^k·C(60,k)·C(n-1,k-1) +
 *            2^60·C(n+29,59) (with a(0)=1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    static seq_t pow2[61];
    static seq_t binom60[61];
    static bool init = false;

    if (!init) {
        pow2[0] = 1;
        for (idx_t i = 1; i <= 60; ++i)
            pow2[i] = pow2[i - 1] * (seq_t)2;

        binom60[0] = 1;
        for (idx_t i = 1; i <= 60; ++i)
            binom60[i] = binomial((idx_t)60, i);

        init = true;
    }

    if (n == 0)
        return (seq_t)1;

    seq_t result = 0;

    idx_t maxk = 60;
    if (maxk > n)
        maxk = n;

    for (idx_t k = 1; k <= maxk; ++k) {
        seq_t bm1k1 = binomial(n - 1, k - 1);
        seq_t term   = pow2[k] * binom60[k] * bm1k1;
        result += term;
    }

    result += pow2[60] * binomial(n + 29, 59);

    return result;
}
