/*
 * A212441  Numbers whose binary representations have odd curling numbers, cf.
 *          A181935.
 * https://oeis.org/A212441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth nonnegative integer whose binary representation has
 *            an odd curling number. The implementation enumerates integers
 *            sequentially, computing their curling number parity directly
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static bool odd_curling(seq_t v)
{
    /* Curling number of 0 is 1 (odd). */
    if (v == 0) return true;

    /* Number of bits in v. */
    idx_t bits = bitlen_seq_t(v);

    /* Find the maximum k such that v = X Y^k with a non‑empty Y. */
    idx_t maxk = 1;
    for (idx_t l = 1; l <= bits; ++l) {
        seq_t mask    = (((seq_t)1 << l) - 1);  /* lower l bits */
        seq_t pattern = v & mask;
        seq_t temp    = v;
        idx_t k = 0;
        while (true) {
            if ((temp & mask) != pattern) break;
            ++k;
            temp >>= l;
            /* Stop if the remaining part contains fewer than l bits
               for another full repetition of Y. */
            if (bitlen_seq_t(temp) < l) break;
        }
        if (k > maxk) maxk = k;
    }
    return (maxk & 1) != 0;
}

seq_t a(idx_t n)
{
    /* Offset 1: a(1)=0. */
    if (n == 0) return 0;

    seq_t val = 0;          /* start with 0 */
    idx_t  cnt = 1;         /* a(1) = 0 already counted */

    while (cnt < n) {
        ++val;
        if (odd_curling(val)) ++cnt;
    }
    return val;
}
