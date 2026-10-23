/*
 * A035807  Coordination sequence for lattice D*_44 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(m) = sum_{k=0..44} 2^k * binomial(44,k) * binomial(m-1,k-1) +
 *            2^44 * binomial((44+2*m)/2-1, 43) Offset: 0 (so a(0) is the
 *            first term listed)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 by definition */
        return (seq_t)1;

    const idx_t N = 44;
    seq_t sum = 0;

    /* first sum over k */
    for (idx_t k = 0; k <= N; ++k) {
        seq_t term = pow2(k);                            /* 2^k */

        term *= binomial(N, k);                          /* * C(44,k) */

        seq_t c = 0;
        if (k > 0)                                       /* C(n-1, k-1) */
            c = binomial(n - 1, k - 1);

        term *= c;                                       /* * C(n-1,k-1) */
        sum += term;
    }

    /* second part: 2^44 * C((44+2*n)/2-1, 43) */
    seq_t second = 0;
    idx_t s = n + 21; /* (44+2*n)/2 - 1 = 22 + n - 1 = n+21 */
    if (s >= 43)
        second = pow2(44) * binomial(s, 43);

    return sum + second;
}
