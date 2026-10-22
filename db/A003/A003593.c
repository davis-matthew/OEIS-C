/*
 * A003593  Numbers of the form 3^i*5^j with i, j >= 0.
 * https://oeis.org/A003593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   merge-based generation (Hamming/Dijkstra style) of numbers
 *            3^i*5^j in increasing order: a(1) = 1, and each next term is
 *            min(3*a(i3), 5*a(i5))
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 157, u64 n <= 591,
 *            u128 n <= 2294, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    idx_t i3 = 0, i5 = 0, k;
    seq_t next3, next5, next, result;
    bool ok3, ok5;

    seq[0] = 1;
    next3 = 3; ok3 = true;
    next5 = 5; ok5 = true;

    for (k = 1; k < n; k++) {
        if (!ok3 && !ok5) {
            /* remaining terms do not fit in seq_t */
            seq[k] = seq[k - 1];
            continue;
        }
        if (ok3 && (!ok5 || next3 < next5)) next = next3;
        else next = next5;
        seq[k] = next;

        if (ok3 && next == next3) {
            i3++;
            next3 = seq[i3] * 3;
            if (next3 / 3 != seq[i3]) ok3 = false;
        }
        if (ok5 && next == next5) {
            i5++;
            next5 = seq[i5] * 5;
            if (next5 / 5 != seq[i5]) ok5 = false;
        }
    }

    result = seq[n - 1];
    free(seq);
    return result;
}
