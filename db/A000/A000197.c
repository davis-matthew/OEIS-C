/*
 * A000197  a(n) = (n!)!.
 * https://oeis.org/A000197
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n!)! = A000142(A000142(n))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    uint64_t m = 1, k;
    seq_t r = 1;

    for (k = 2; k <= n; k++)
        m *= k;

    k = 2;
    /* multiply two factors at a time to halve the number of value operations */
    while (k + 1 <= m) {
        r *= (seq_t)(k * (k + 1));
        if (r == 0) /* fixed-width wraparound: all further terms stay 0 */
            return r;
        k += 2;
    }
    if (k <= m)
        r *= (seq_t)k;
    return r;
}
