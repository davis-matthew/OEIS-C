/*
 * A124763  Number of non-rises (levels or falls) for compositions in standard
 *          order.
 * https://oeis.org/A124763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = number of non‑rises in the composition at index n in
 *            standard order
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* positions of set bits in n (1‑based) */
    idx_t pos[64];
    int    m = 0;
    idx_t  bits   = n;
    idx_t  bitpos = 1;

    while (bits) {
        if (bits & 1) pos[m++] = bitpos;
        bits >>= 1;
        ++bitpos;
    }

    /* count non‑rises in the composition */
    seq_t  count = 0;
    idx_t  prev  = 0;
    int    first = 1;

    for (int i = m; i >= 1; --i) {
        idx_t part;
        if (i == 1) part = pos[0];
        else       part = pos[i-1] - pos[i-2];

        if (first) {
            prev  = part;
            first = 0;
        } else {
            if (prev >= part) ++count;
            prev = part;
        }
    }

    return (seq_t)count;
}
