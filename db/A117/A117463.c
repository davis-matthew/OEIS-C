/*
 * A117463  Result of the final iteration of doubling in A117462 to equal or
 *          exceed n.
 * https://oeis.org/A117463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, less
 * Formula:   a(n) = s * 2^k, where s = sum of decimal digits of (n+1), k =
 *            max(1, ceil(log₂( (n+1)/s )))
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   sum_of_digits_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A117462(n) = minimal k≥0 with s·2^k ≥ n+1,
   and the sequence multiplies by at least one doubling. */

#include <stdint.h>
#include "../utils/sum_of_digits_uint64.h"

/* smallest integer ≥ log₂(x)  (x > 0).  For x == 0 returns 0. */
static int ceil_log2_uint64(uint64_t x)
{
    if (x <= 1) return 0;
    int l = 0;
    uint64_t v = x - 1;
    while (v) {
        v >>= 1;
        ++l;
    }
    return l;
}

seq_t a(idx_t n)
{
    /* target = n + 1 */
    uint64_t target = (uint64_t)n + 1;

    /* s = sum of digits of target */
    uint64_t s = sum_of_digits_uint64(target);

    /* r = ceil(target / s) = (target + s - 1) / s
       Using (n + s) / s gives the same integer result. */
    uint64_t r = ((uint64_t)n + s) / s;  /* ceil( (n+1)/s ) */

    /* k = ceil(log₂(r)) */
    int k = ceil_log2_uint64(r);
    if (k < 1) k = 1;  /* guarantee at least one doubling */

    /* a(n) = s * 2^k  */
    seq_t result = (seq_t)s;
    while (k--) result <<= 1;

    return result;
}
