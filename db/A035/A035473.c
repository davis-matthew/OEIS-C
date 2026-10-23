/*
 * A035473  Coordination sequence for lattice D*_8 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum_{k=1..8} 2^k * binomial(8, k) * binomial(n-1, k-1) +
 *            2^8 * binomial(n+3, 7), with a(0)=1, for n ≥ 0
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 32, u64 n <= 783, u128 n <= 1000,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

/* Pre‑computed binomial coefficients C(8, k). */
static const seq_t BINOM8[9] = {
    1ULL,  8ULL, 28ULL, 56ULL, 70ULL,
   56ULL, 28ULL,  8ULL, 1ULL
};

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition. */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;

    /* First summation term.  k runs from 1 to 8 but can be stopped
       when k > n because binomial(n-1,k-1) will then be zero. */
    for (idx_t k = 1; k <= 8 && k <= n; ++k) {
        seq_t term = BINOM8[k];                    /* C(8,k)   */
        term *= binomial(n - 1, k - 1);           /* C(n-1,k-1) */
        term *= ((seq_t)1 << k);                  /* 2^k      */
        sum += term;
    }

    /* Second term: 2^8 * binomial(n+3, 7)  (since (8+2n)/2-1 = n+3). */
    seq_t second = binomial(n + 3, 7);
    second *= ((seq_t)1 << 8);                    /* 2^8 = 256 */

    return sum + second;
}
