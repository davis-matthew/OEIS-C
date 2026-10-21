/*
 * A053646  Distance to nearest power of 2.
 * https://oeis.org/A053646
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min(n-2^⌊log₂(n)⌋, 2·2^⌊log₂(n)⌋-n)
 * Range:     u8 n <= 766, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n <= 1) return 0;                     /* a(1) = 0 */

    /* Find k = floor(log2(n)) by inspecting bits. */
    unsigned int k = 0;
    for (int i = 63; i >= 0; --i) {
        if (n & ((idx_t)1ULL << i)) {
            k = (unsigned int)i;
            break;
        }
    }

    idx_t p = (idx_t)1ULL << k;                /* p = 2^k <= n */
    seq_t diff1 = (seq_t)n - (seq_t)p;         /* n - p   */
    seq_t diff2 = (seq_t)p - diff1;            /* p - diff1 = 2p - n without overflow */

    return diff1 < diff2 ? diff1 : diff2;      /* choose the smaller distance */
}
