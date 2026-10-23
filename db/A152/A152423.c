/*
 * A152423  A variation of the Josephus problem, removing every other person,
 *          starting with person 1; a(n) is the last person remaining.
 * https://oeis.org/A152423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n - 2^ceil(log2(n))
 * Range:     u8 n <= 255, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   floor_log2_uint64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* Handle the optional n = 0 case (ignored in OEIS). */
    if (n == 0)
        return (seq_t)0;

    /* m = floor(log2(n)), i.e. largest k with 2^k ≤ n. */
    idx_t m = floor_log2_uint64(n);

    /* powp = 2^m as seq_t. */
    seq_t powp = pow2(m);

    /* sn = n converted to seq_t. */
    seq_t sn = (seq_t)n;

    /* If n is a power of two then ceil(log2(n)) = m. */
    if (sn == powp)
        return sn;

    /* Otherwise ceil(log2(n)) = m+1, so 2^ceil = 2*powp. */
    return sn + sn - (powp << 1);
}
