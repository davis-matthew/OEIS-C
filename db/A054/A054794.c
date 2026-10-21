/*
 * A054794  Numbers of form 5+n^2+n or 5+2*n^2.
 * https://oeis.org/A054794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A054794: merge of 5 + n² + n and 5 + 2n², sorted and unique. */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;                /* undefined for n = 0, safe return */

    idx_t i = 0, j = 0;                 /* pointers into the two sequences */
    for (idx_t count = 1; count <= n; ++count) {
        /* compute the next candidate from each sequence using seq_t arithmetic */
        seq_t v1 = 5 + (seq_t)i * ((seq_t)i + 1);                 /* 5 + n² + n  */
        seq_t v2 = 5 + (seq_t)2 * (seq_t)j * (seq_t)j;            /* 5 + 2n²     */

        seq_t next;
        if (v1 < v2) {
            next = v1;
            ++i;
        } else if (v2 < v1) {
            next = v2;
            ++j;
        } else {                /* duplicate – skip once */
            next = v1;
            ++i;
            ++j;
        }

        if (count == n) return next;    /* nth term found */
    }

    return 0;    /* control should never reach here */
}
