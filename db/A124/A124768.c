/*
 * A124768  Number of strictly increasing runs for compositions in standard
 *          order.
 * https://oeis.org/A124768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) = number of strictly increasing runs in the composition *
 *            of n defined by the standard order (A066099). For n > 0: * 1.
 *            Write n in binary, read the bits from least‑significant to *
 *            most‑significant; the positions (starting at 1) of the 1‑bits *
 *            are recorded in increasing order. * 2. Prepend...
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* Step 1: positions of 1‑bits (LSB = position 1) */
    idx_t posList[64];
    idx_t cnt = 0;
    idx_t pos = 1;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1) {
            posList[cnt++] = pos;
        }
        pos++;
        tmp >>= 1;
    }

    /* Step 2: compute successive differences */
    seq_t diffs[64];
    idx_t prev = 0;
    for (idx_t i = 0; i < cnt; ++i) {
        diffs[i] = (seq_t)(posList[i] - prev);
        prev = posList[i];
    }

    /* Step 3: count strictly increasing runs in the reversed list */
    seq_t runs = 1;
    if (cnt > 1) {
        seq_t prevPart = diffs[cnt - 1];
        for (int i = (int)cnt - 2; i >= 0; --i) {
            seq_t part = diffs[i];
            if (part <= prevPart) runs++;
            prevPart = part;
        }
    }
    return runs;
}
