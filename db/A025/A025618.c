/*
 * A025618  Numbers of form 4^i*6^j, with i, j >= 0.
 * https://oeis.org/A025618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 32, u32 n <= 113, u64 n <= 424,
 *            u128 n <= 1641, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers of the form 4^i * 6^j (i,j >= 0).
   Computed by iteratively merging multiples of 4 and 6 to generate
   an increasing list of such numbers. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* offset 1 – a(1) = 1 */
    if (n == 0) return (seq_t)0;

    static seq_t *seq = NULL;
    static size_t length = 0, capacity = 0;
    static idx_t i4 = 0, i6 = 0;          /* pointers for 4·seq[i4] and 6·seq[i6] */

    idx_t idx = n - 1;                    /* 0‑based index */

    if (length == 0) {
        capacity = 16;
        seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
        seq[0] = (seq_t)1;
        length = 1;
    }

    while ((size_t)idx >= length) {
        if (length >= capacity) {
            capacity *= 2;
            seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
        }

        seq_t val4 = seq[i4] * ((seq_t)4);
        seq_t val6 = seq[i6] * ((seq_t)6);

        seq_t next;
        int inc4 = 0, inc6 = 0;

        if (val4 < val6) {
            next = val4;
            inc4 = 1;
        } else if (val6 < val4) {
            next = val6;
            inc6 = 1;
        } else {            /* val4 == val6 */
            next = val4;
            inc4 = inc6 = 1;
        }

        seq[length++] = next;
        if (inc4) ++i4;
        if (inc6) ++i6;
    }

    return seq[(size_t)idx];
}
