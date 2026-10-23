/*
 * A177837  a(n) = binomial(n^3-2, n-2).
 * https://oeis.org/A177837
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n^3-2, n-2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;                    /* offset: a(2) is first term */

    /* n^3 easily fits into 64‑bit for the intended range (up to ~10⁴) */
    idx_t n3 = (idx_t)n * (idx_t)n * (idx_t)n;
    idx_t num = n3 - 2;               /* upper argument of the binomial */
    idx_t k   = n - 2;                /* lower argument of the binomial */

    return binomial(num, k);
}
