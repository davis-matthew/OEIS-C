/*
 * A003596  Numbers of the form 2^i * 11^j.
 * https://oeis.org/A003596
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   merge the 2*a and 11*a candidates in increasing order
 *            (Hamming-style), starting from a(1) = 1
 * Range:     u8 n <= 15, u16 n <= 48, u32 n <= 169, u64 n <= 634,
 *            u128 n <= 2451, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t *seq = (seq_t *)calloc((size_t)n, sizeof(seq_t));
    seq[0] = 1;

    idx_t i2 = 0, i11 = 0;
    idx_t k;

    for (k = 1; k < n; k++) {
        seq_t x2 = seq[i2];
        seq_t x11 = seq[i11];
        seq_t next2 = x2 * 2;
        seq_t next11 = x11 * 11;
        bool ov2 = (next2 / 2 != x2);
        bool ov11 = (next11 / 11 != x11);

        if (ov2 && ov11) {
            /* term does not fit in seq_t; value is irrelevant */
            seq[k] = next2;
            i2++;
            i11++;
            continue;
        }
        if (ov11 || (!ov2 && next2 < next11)) {
            seq[k] = next2;
            i2++;
        } else if (ov2 || next11 < next2) {
            seq[k] = next11;
            i11++;
        } else {
            seq[k] = next2;
            i2++;
            i11++;
        }
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
