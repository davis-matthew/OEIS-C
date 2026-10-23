/*
 * A035814  Coordination sequence for lattice D*_58 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{k=0..58} 2^k * C(58,k) * C(n-1,k-1) + 2^58 *
 *            C((58+2n)/2-1,57)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (Here C(a,b) denotes the binomial coefficient, defined to be 0 if b<0 or b>a.) */

#include <stdbool.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    const idx_t N = 58;               /* fixed lattice dimension */
    static seq_t pow2k[59];           /* pow2k[k] = 2^k                  */
    static seq_t binom_N_k[59];       /* binom_N_k[k] = C(58,k)           */
    static bool initialized = false;

    if (!initialized) {
        for (idx_t k = 0; k <= N; ++k) {
            pow2k[k] = (seq_t)1 << k;                 /* 2^k          */
            binom_N_k[k] = binomial(N, k);           /* C(58,k)      */
        }
        initialized = true;
    }

    /* Handle the special case n = 0 separately (a(0)=1). */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    /* First summation: k = 1..58  (k = 0 term is zero for n>0). */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t c58k = binom_N_k[k];
        if (c58k == 0)
            continue;
        seq_t comb = binomial(n - 1, k - 1);   /* C(n-1, k-1) */
        if (comb == 0)
            continue;
        seq_t term = pow2k[k] * c58k;
        term *= comb;
        sum += term;
    }

    /* Second term: 2^58 * C(n + 28, 57)   (since (58+2n)/2-1 = n+28). */
    /* Only add if the upper argument is at least 57. */
    if (n + 28 >= 57) {
        seq_t part2 = pow2k[N] * binomial(n + 28, 57);
        sum += part2;
    }

    return sum;
}
