/*
 * A118967  If n doesn't occur among the first (n-1) terms of the sequence,
 *          then a(n) = 2n. If n occurs among the first (n-1) terms of the
 *          sequence, then a(n) = n/2.
 * https://oeis.org/A118967
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(2^m) = 2^(m-(-1)^m); a(k*2^m)=k*2^(m+(-1)^m) for odd k>1
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* a(1) is defined explicitly */
    if (n == 1u)
        return (seq_t)1;

    /* Find exponent of 2 (m) and odd part (k) */
    idx_t m = 0;
    idx_t temp = n;
    while ((temp & 1u) == 0u) {
        ++m;
        temp >>= 1;
    }
    /* temp is now the odd part k */
    seq_t k = (seq_t)temp;

    /* Compute exponent for the power of two in the result */
    idx_t exp;
    bool is_power = (k == (seq_t)1);
    if (is_power) {
        /* For powers of two: shift = +1 when m odd, -1 when m even */
        exp = (m & 1u) ? m + 1u : m - 1u;
    } else {
        /* For non‑powers: shift = -1 when m odd, +1 when m even */
        exp = (m & 1u) ? m - 1u : m + 1u;
    }

    /* Compute the result using the pre‑existing pow2 function */
    seq_t pow_val = pow2(exp);
    return is_power ? pow_val : k * pow_val;
}
