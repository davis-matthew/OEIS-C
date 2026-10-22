/*
 * A003598  Numbers of the form 5^i * 11^j.
 * https://oeis.org/A003598
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1; a(n) = min(5*a(i), 11*a(j)) over the smallest such
 *            products exceeding a(n-1) (two-pointer merge, Hamming-style)
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 75, u64 n <= 280,
 *            u128 n <= 1065, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t *seq = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
    idx_t i5 = 0, i11 = 0, i;
    seq_t result;
    seq[0] = 1;

    for (i = 1; i < n; i++) {
        seq_t next5 = seq[i5] * (seq_t)5;
        seq_t next11 = seq[i11] * (seq_t)11;
        bool ok5 = (next5 / (seq_t)5 == seq[i5]);
        bool ok11 = (next11 / (seq_t)11 == seq[i11]);

        if (!ok5 && !ok11) {
            /* true value no longer fits in seq_t */
            seq[i] = next5;
            i5++;
            continue;
        }
        if (ok5 && (!ok11 || next5 <= next11)) {
            seq[i] = next5;
            i5++;
            if (ok11 && next5 == next11) i11++;
        } else {
            seq[i] = next11;
            i11++;
        }
    }

    result = seq[n - 1];
    free(seq);
    return result;
}
