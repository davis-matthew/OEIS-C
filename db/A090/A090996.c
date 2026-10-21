/*
 * A090996  Number of leading 1's in binary expansion of n.
 * https://oeis.org/A090996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of consecutive leading 1's in the binary
 *            representation of n (a(0) = 0, a(1) = 1, a(2) = 1, a(3) = 2,
 *            a(14) = 3, a(15) = 4, …)
 * Range:     u8 n <= 7685, u16 n <= 7685, u32 n <= 7685, u64 n <= 7685,
 *            u128 n <= 7685, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Find the position of the most significant ‘1’ bit (0‑based). */
    /* idx_t is uint64_t, so builtin clz works. */
    int msb = 63 - __builtin_clzll((uint64_t)n);   /* floor(log2(n)) */

    seq_t count = (seq_t)1;   /* the MSB itself is always 1 */

    /* Count further consecutive 1's towards the LSB. */
    while (msb > 0 && ((n >> (msb - 1)) & 1ULL)) {
        ++count;
        --msb;
    }

    return count;
}
