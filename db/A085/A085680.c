/*
 * A085680  Size of largest code of length n and constant weight 2 that can
 *          correct a single adjacent transposition.
 * https://oeis.org/A085680
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, more, changed
 * Formula:   a(n) = d(n-2) - d(n-3) + d(n-4) - d(n-12) + d(n-13) where d(m) =
 *            Sum_{k=0}^{⌊m/5⌋} (m - 5k + 1)
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   d, d_nonneg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/d.h"
#include "../utils/d_nonneg.h"

seq_t a(idx_t n)
{
    /* Offset is 2 – values below 2 are undefined */
    if (n < 2) return 0;   /* just in case */

    seq_t result = 0;
    intmax_t k;

    k = (intmax_t)n - 2;
    result += d_nonneg(k);

    k = (intmax_t)n - 3;
    result -= d_nonneg(k);

    k = (intmax_t)n - 4;
    result += d_nonneg(k);

    k = (intmax_t)n - 12;
    result -= d_nonneg(k);

    k = (intmax_t)n - 13;
    result += d_nonneg(k);

    return result;
}
