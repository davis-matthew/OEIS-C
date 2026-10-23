/*
 * A164150  Number of binary strings of length n with equal numbers of 0000
 *          and 0101 substrings.
 * https://oeis.org/A164150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of binary strings of length n with equal numbers
 *            of 0000 and 0101 substrings
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   delta_from_state_and_bit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/delta_from_state_and_bit.h"

static const int STATE_COUNT = 8;     /* 3‑bit last context (000..111) */

seq_t a(idx_t n)
{
    /* Handle short lengths directly. */
    if (n == 0) return (seq_t)1;   /* empty string */
    if (n == 1) return (seq_t)2;   /* 0,1 */
    if (n == 2) return (seq_t)4;   /* 00,01,10,11 */
    if (n == 3) return (seq_t)8;   /* all 3‑bit strings, no substrings of length 4 */

    /* Difference range [-n .. +n] => array offset = n. */
    size_t diff_size = (size_t)(2 * n + 1);   /* inclusive end */
    size_t offset    = (size_t)n;           /* zero difference index */

    /* Allocate current and next DP tables. */
    size_t table_size = (size_t)STATE_COUNT * diff_size;
    seq_t *cur  = (seq_t *)calloc(table_size, sizeof(seq_t));
    seq_t *next = (seq_t *)calloc(table_size, sizeof(seq_t));
    if (!cur || !next) {   /* allocation failed; return 0 as fallback */
        free(cur); free(next);
        return (seq_t)0;
    }

    /* Initialize: after 3 bits, every 3‑bit suffix is possible with diff 0. */
    for (int s = 0; s < STATE_COUNT; ++s) {
        cur[(size_t)s * diff_size + offset] = (seq_t)1;
    }

    idx_t cur_len = 3;
    while (cur_len < n) {
        /* Zero the next buffer efficiently. */
        memset(next, 0, table_size * sizeof(seq_t));

        for (int s = 0; s < STATE_COUNT; ++s) {
            size_t base = (size_t)s * diff_size;
            for (size_t d = 0; d < diff_size; ++d) {
                seq_t val = cur[base + d];
                if (!val) continue;           /* skip zero counts */

                /* Two possible next bits. */
                for (int bit = 0; bit < 2; ++bit) {
                    int s2   = ((s << 1) | bit) & 7;
                    int d2   = (int)d + delta_from_state_and_bit(s, bit);
                    if (d2 < 0 || d2 >= (int)diff_size) continue; /* safety */

                    size_t idx = (size_t)s2 * diff_size + (size_t)d2;
                    next[idx] += val;          /* wrap automatically for unsigned */
                }
            }
        }

        /* Swap current and next for next iteration. */
        seq_t *tmp  = cur;
        cur        = next;
        next       = tmp;
        ++cur_len;
    }

    /* Sum over all last‑3‑bit states for zero difference. */
    seq_t result = (seq_t)0;
    for (int s = 0; s < STATE_COUNT; ++s) {
        result += cur[(size_t)s * diff_size + offset];
    }

    free(cur);
    free(next);
    return result;
}
