/*
 * A024499  a(n) = [ C(2n,n)/(n-1) ].
 * https://oeis.org/A024499
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n,n)/(n-1)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 26 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* term undefined for n < 2 */
    /* compute binomial coefficient C(2n,n) */
    seq_t bin = binomial((idx_t)(2 * n), n);
    /* divide by (n-1) */
    return bin / ((seq_t)n - 1);
}
