/*
 * A038166  G.f.: 1/((1-x)*(1-x^2))^6.
 * https://oeis.org/A038166
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = binomial(k+8,8)*(2k+9)*(8k^2+72k+55)/(5*9*11) a(2k+1)=
 *            2*binomial(k+9,9)*(8k^2+80k+165)/(5*11) Closed‑form evaluation
 *            for even and odd indices
 * Range:     u8 none, u16 n <= 3, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if ((n & 1ULL) == 0) {                 /* even index: a(2k) */
        idx_t k = n >> 1;
        seq_t bin = binomial(k + 8, 8);
        seq_t term = bin;
        term *= (seq_t)(2 * k + 9);
        term *= (seq_t)(8 * k * k + 72 * k + 55);
        term /= 495;                       /* 5*9*11 */
        return term;
    } else {                                /* odd index: a(2k+1) */
        idx_t k = (n - 1) >> 1;
        seq_t bin = binomial(k + 9, 9);
        seq_t term = bin;
        term *= (seq_t)(8 * k * k + 80 * k + 165);
        term *= 2;
        term /= 55;                         /* 5*11 */
        return term;
    }
}
