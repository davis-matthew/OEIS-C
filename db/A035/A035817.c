/*
 * A035817  Coordination sequence for lattice D*_64 (with edges defined by l_1
 *          norm = 1).
 * https://oeis.org/A035817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A035817
   a(m) = sum_{k=0}^{min(64,m)} 2^k·C(64,k)·C(m-1,k-1) + 2^64·C(m+31,63);
   a(0) = 1.  (For m ≥ 1 the k = 0 term vanishes.)  */
#include <stdbool.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t m)
{
    /* Handle the offset term */
    if (m == 0)
        return (seq_t)1;

    /* Pre‑compute 2^k and C(64,k) once. */
    static seq_t pow2k[65];
    static seq_t binom64k[65];
    static bool init = false;

    if (!init) {
        for (idx_t k = 0; k <= 64; ++k) {
            pow2k[k]    = pow2(k);          /* 2^k   */
            binom64k[k] = binomial(64, k);  /* C(64,k) */
        }
        init = true;
    }

    seq_t result = 0;

    /* First sum: k = 1 … min(64, m)  (k = 0 contributes 0 when m ≥ 1) */
    idx_t limit = m < 64 ? m : 64;
    for (idx_t k = 1; k <= limit; ++k) {
        seq_t c1 = binom64k[k];                     /* C(64,k) */
        seq_t c2 = binomial(m - 1, k - 1);          /* C(m-1,k-1) */
        seq_t term = pow2k[k] * (c1 * c2);
        result += term;
    }

    /* Second part: 2^64 · C(m+31,63)  */
    seq_t bin = binomial(m + 31, 63);
    if (bin != 0) {
        seq_t two64 = pow2(64);          /* 2^64 */
        result += two64 * bin;
    }

    return result;
}
