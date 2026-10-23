/*
 * A101348  Arises from a particular cyclic transformation of the floretion -
 *          .5'i - .5i' - .5'ij' - .5'ik' + .5'ji' + .5'ki'.
 * https://oeis.org/A101348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 89, u16 n <= 89, u32 n <= 89, u64 n <= 89, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-3) - a(n-4) - 2*a(n-6) + 2*a(n-7)
 *               + 2*a(n-9) - 2*a(n-10) - a(n-12) + a(n-13) + a(n-15) - a(n-16)
 *   for n > 15.
 * Initial values:
 *   a(0)=0, a(1)=1, a(2)=1, a(3)=2, a(4)=2, a(5)=0, a(6)=3, a(7)=0,
 *   a(8)=0, a(9)=0, a(10)=0, a(11)=4, a(12)=0, a(13)=5, a(14)=5, a(15)=6.
 */
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    /* Pre‑computed first 16 terms (indices 0..15). */
    static const seq_t init[16] = {
        0, 1, 1, 2, 2, 0, 3, 0,
        0, 0, 0, 4, 0, 5, 5, 6
    };

    if (n < 16) return init[n];

    /* Allocate array for all values up to n. */
    seq_t *seq = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!seq) return 0;  /* allocation failed – return 0 as a safe fallback */

    /* Copy initial values. */
    memcpy(seq, init, sizeof(init));

    /* Compute remaining terms using the recurrence. */
    for (idx_t i = 16; i <= n; ++i) {
        seq_t pos = seq[i - 1] + seq[i - 3];
        pos += (seq[i - 7] << 1);
        pos += (seq[i - 9] << 1);
        pos += seq[i - 13];
        pos += seq[i - 15];

        seq_t neg = seq[i - 4];
        neg += (seq[i - 6] << 1);
        neg += (seq[i - 10] << 1);
        neg += seq[i - 12];
        neg += seq[i - 16];

        seq[i] = pos - neg;   /* safe: pos ≥ neg for this sequence */
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
