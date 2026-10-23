/*
 * A117462  The number of doubling steps of SOD(n) to exceed or equal n.
 * https://oeis.org/A117462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = max(1, ceil(log₂(n / SOD(n))) )
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   bitlen_seq_t, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_sum.h"
#include "../utils/bitlen_seq_t.h"

seq_t a(idx_t n)
{
    /* Convert n to seq_t */
    seq_t n_seq = (seq_t)n;

    /* Compute sum of decimal digits of n */
    seq_t s = digit_sum(n_seq);

    /* If the sum already meets or exceeds n, we need exactly one doubling */
    if (s >= n_seq) {
        return (seq_t)1;
    }

    /* Compute ceil(n / s) */
    seq_t r = (n_seq + s - 1) / s;          /* r >= 2 */

    /* Compute ceil(log₂(r)) using bit length of r-1 */
    seq_t k = (seq_t)bitlen_seq_t(r - 1);    /* k >= 1 */

    return k;
}
