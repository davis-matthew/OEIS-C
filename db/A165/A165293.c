/*
 * A165293  Inverse of A038303, and generalization of A130595.
 * https://oeis.org/A165293
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   Let n >= 1. Set idx = n - 1 r = floor((sqrt(8*idx + 1) - 1) / 2)
 *            prev = r*(r+1) / 2 k = idx - prev Then a(n) = binom(r, k) *
 *            10^(r-k) * (-1)^k (binom and pow10 are provided helper
 *            functions.)
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binom, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow10.h"
#include "../utils/_floor_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* Adjust for one-based index */
    idx_t idx = n - 1;

    /* Compute r via integer sqrt */
    /* tmp = 8*idx + 1 can safely fit into 64-bit for required n */
    idx_t tmp = (idx << 3) + (idx >> 61) + 1;  /* (idx*8)+1 */
    idx_t s   = _floor_isqrt_u64(tmp);
    idx_t r   = (s - 1) >> 1;

    /* previous cumulative count */
    idx_t prev = (r * (r + 1)) >> 1;

    /* position within the row */
    idx_t k   = idx - prev;

    /* compute binomial coefficient and power of 10 */
    seq_t bin   = binom(r, k);
    seq_t pow10v = pow10(r - k);

    /* combine with sign (-1)^k */
    seq_t result = bin * pow10v;
    if (k & 1)
        result = -result;

    return result;
}
