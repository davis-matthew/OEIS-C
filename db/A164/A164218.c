/*
 * A164218  Number of binary strings of length n with equal numbers of 00010
 *          and 01100 substrings.
 * https://oeis.org/A164218
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A164218
 *  Count binary strings of length n whose number of 00010 substrings
 *  equals the number of 01100 substrings.
 *  The program uses a dynamic‑programming automaton over the last
 *  four bits and the difference between the two pattern counts.
 */
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define PAT_A 0b00010
#define PAT_B 0b01100

seq_t a(idx_t n)
{
    /* The difference can range from -n to +n.
       We shift it by +n to obtain a non‑negative index. */
    size_t diff_size = (size_t)(2 * n + 1);
    size_t offset    = (size_t)n;

    /* Two DP layers: current and next. */
    seq_t *cur  = (seq_t *)calloc(16 * diff_size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(16 * diff_size, sizeof(seq_t));
    if (!cur || !next) {
        /* Allocation failure – fall back to zero value. */
        free(cur);
        free(next);
        return (seq_t)0;
    }

    /* Start with an empty string: last four bits = 0, diff = 0. */
    cur[0 * diff_size + offset] = (seq_t)1;

    for (idx_t len = 0; len < n; ++len) {
        /* Clear next layer. */
        memset(next, 0, sizeof(seq_t) * 16 * diff_size);

        /* At current length len, diff can be within [-len, +len]. */
        int d_min = -(int)len;
        int d_max =  (int)len;

        for (int s = 0; s < 16; ++s) {
            size_t base = (size_t)s * diff_size;
            for (int d = d_min; d <= d_max; ++d) {
                seq_t val = cur[base + (size_t)(d + offset)];
                if (val == (seq_t)0) continue;

                for (int bit = 0; bit < 2; ++bit) {
                    int new_s = ((s << 1) | bit) & 0xF;
                    int delta = 0;
                    if (len + 1 >= 5) {
                        int patt = ((s << 1) | bit) & 0x1F; /* 5‑bit window */
                        if (patt == PAT_A)      delta =  1;
                        else if (patt == PAT_B) delta = -1;
                    }
                    int new_d = d + delta;
                    size_t idx = new_s * diff_size + (size_t)(new_d + offset);
                    next[idx] += val;
                }
            }
        }

        /* Swap layers for next iteration. */
        seq_t *tmp = cur; cur = next; next = tmp;
    }

    /* Sum all strings whose final difference is zero. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < 16; ++s) {
        result += cur[s * diff_size + offset];
    }

    free(cur);
    free(next);
    return result;
}
