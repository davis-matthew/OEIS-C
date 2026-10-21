/*
 * A204293  Pascal's triangle interspersed with rows of zeros, and the rows of
 *          Pascal's triangle are interspersed with zeros.
 * https://oeis.org/A204293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is 0 unless n can be written as n = m(2m+1)+2k with 0 ≤ k ≤
 *            m, in which case a(n) = C(m,k). (This follows from the
 *            definition of T(n,k) and the interleaving of zeros as described
 *            in OEIS A204293.)
 * Range:     u8 n <= 260, u16 n <= 756, u32 n <= 2518, u64 n <= 5049,
 *            u128 n <= 5049, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"   /* provides seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    for (idx_t m = 0;; ++m) {
        idx_t base = m * (2 * m + 1);         /* m*(2m+1) */
        if (base > n) break;                 /* base grows monotonically */

        idx_t diff = n - base;                /* remaining part */
        if ((diff & 1) == 0) {               /* need diff even */
            idx_t k = diff >> 1;              /* k = diff/2 */
            if (k <= m) {
                return binomial(m, k);        /* return C(m,k) */
            }
        }
    }
    return 0;    /* if no (m,k) found, the entry is 0 */
}
