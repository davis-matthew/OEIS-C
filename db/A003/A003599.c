/*
 * A003599  Numbers of the form 7^i*11^j.
 * https://oeis.org/A003599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- merge the multiples 7*a(k) and 11*a(k) in ascending
 *            order (Hamming-style), a(1) = 1
 * Range:     u8 n <= 6, u16 n <= 19, u32 n <= 64, u64 n <= 232, u128 n <= 886,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *seq = NULL;
    static idx_t seq_cap = 0;
    static idx_t seq_len = 0;
    static idx_t i7 = 1, i11 = 1;

    if (n == 0) return 0;

    if (seq == NULL) {
        seq_cap = 1024;
        seq = (seq_t *)malloc((size_t)(seq_cap + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= seq_cap; k++) seq[k] = 0;
        seq[1] = 1;
        seq_len = 1;
        i7 = 1;
        i11 = 1;
    }

    if (n > seq_cap) {
        idx_t newcap = seq_cap;
        while (newcap < n) newcap *= 2;
        seq_t *tmp = (seq_t *)malloc((size_t)(newcap + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= newcap; k++) tmp[k] = 0;
        for (idx_t k = 0; k <= seq_len; k++) tmp[k] = seq[k];
        free(seq);
        seq = tmp;
        seq_cap = newcap;
    }

    while (seq_len < n) {
        seq_t b7 = seq[i7], b11 = seq[i11];
        seq_t c7 = b7 * 7;
        seq_t c11 = b11 * 11;
        bool ok7 = (c7 / 7 == b7) && (c7 > b7);
        bool ok11 = (c11 / 11 == b11) && (c11 > b11);
        idx_t pos = seq_len + 1;

        if (!ok7 && !ok11) {
            /* further terms do not fit in seq_t */
            seq[pos] = 0;
        } else if (ok7 && (!ok11 || c7 < c11)) {
            seq[pos] = c7;
            i7++;
        } else if (ok11 && (!ok7 || c11 < c7)) {
            seq[pos] = c11;
            i11++;
        } else {
            seq[pos] = c7;
            i7++;
            i11++;
        }
        seq_len = pos;
    }

    return seq[n];
}
