/*
 * A107777  a(1)=6, a(n) = smallest integer not previously used which contains
 *          a digit from a(n-1).
 * https://oeis.org/A107777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest unused integer that contains a digit from
 *            a(n-1), with a(1) = 6. a(1)=6, a(2)=16, a(3)=1, a(4)=10, a(5)=0,
 *            …
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   digit_mask_local
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/digit_mask_local.h"

/* Sequence computation */
seq_t a(idx_t n)
{
    /* static storage for the sequence */
    static seq_t *seq = NULL;
    static size_t sz = 0;      /* number of terms stored */
    static size_t cap = 0;     /* current capacity of seq */

    if (n == 0) return 0; /* undefined for n=0, return 0 */

    /* initialization on first call */
    if (sz == 0) {
        cap = 64;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;    /* allocation failed – unlikely */
        seq[0] = (seq_t)6;      /* a(1) */
        sz = 1;
    }

    /* compute terms up to the requested index */
    while (sz < n) {
        seq_t prev = seq[sz - 1];
        uint16_t prevMask = digit_mask_local(prev);

        seq_t cand = (seq_t)0;
        for (;;) {
            /* check if cand has already appeared */
            bool used = false;
            for (size_t i = 0; i < sz; ++i) {
                if (seq[i] == cand) {
                    used = true;
                    break;
                }
            }
            if (used) {
                ++cand;
                continue;
            }

            /* does candidate share a digit with prev? */
            if ((prevMask & digit_mask_local(cand)) != 0) {
                /* store the new term */
                if (sz == cap) {
                    cap <<= 1;
                    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
                }
                seq[sz++] = cand;
                break;
            }
            ++cand;
        }
    }

    return seq[n - 1];
}
