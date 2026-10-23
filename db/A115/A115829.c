/*
 * A115829  Integers i such that 10*i XOR 11*i = 21*i.
 * https://oeis.org/A115829
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Property: 10*i XOR 11*i = 21*i  (equivalently 10*i & 11*i == 0) */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t cap = 0;
    static size_t sz  = 0;          /* number of terms stored */
    static seq_t  last_i = 0;       /* next candidate to test */

    size_t target = (size_t)n;

    /* Ensure the table is large enough to contain the target term */
    if (sz <= target) {
        if (!seq) {
            cap = 16;
            seq = (seq_t*)malloc(cap * sizeof(seq_t));
        }

        seq_t i = last_i;
        seq_t zero = 0;   /* 0 of type seq_t */

        while (sz <= target) {
            seq_t x = i * (seq_t)10;
            seq_t y = i * (seq_t)11;

            if ((x & y) == zero) {
                if (sz >= cap) {
                    cap *= 2;
                    seq = (seq_t*)realloc(seq, cap * sizeof(seq_t));
                }
                seq[sz++] = i;
            }
            i++;
        }
        last_i = i;   /* next candidate after the last tested i */
    }

    return seq[target];
}
