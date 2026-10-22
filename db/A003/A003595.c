/*
 * A003595  Numbers of the form 5^i*7^j with i, j >= 0.
 * https://oeis.org/A003595
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   merge the two lists by 5 and by 7: a(1) = 1; a(k) = min(5*a(i5),
 *            7*a(i7)), the smallest such product that is greater than a(k-1)
 * Range:     u8 n <= 9, u16 n <= 26, u32 n <= 91, u64 n <= 340,
 *            u128 n <= 1307, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t *seq = (seq_t *)malloc((size_t)n * sizeof(seq_t));
    seq[0] = (seq_t)1;

    idx_t i5 = 0, i7 = 0;

    for (idx_t k = 1; k < n; k++) {
        seq_t next_5 = seq[i5] * (seq_t)5;
        seq_t next_7 = seq[i7] * (seq_t)7;
        bool ok5 = (next_5 / (seq_t)5 == seq[i5]);
        bool ok7 = (next_7 / (seq_t)7 == seq[i7]);

        seq_t next;
        if (!ok5 && !ok7) {
            /* value no longer fits in seq_t; result undefined anyway */
            next = next_5;
            i5++;
        } else if (!ok7 || (ok5 && next_5 < next_7)) {
            next = next_5;
            i5++;
        } else if (!ok5 || next_7 < next_5) {
            next = next_7;
            i7++;
        } else {
            next = next_5;
            i5++;
            i7++;
        }

        seq[k] = next;
    }

    seq_t result = seq[n - 1];
    free(seq);
    return result;
}
