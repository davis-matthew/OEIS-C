/*
 * A191263  Positions of 1 in A191261.
 * https://oeis.org/A191263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = position of the n-th 1 in the fixed point of the morphism
 *            0 → 01, 1 → 000. The routine expands the word iteratively until
 *            it contains at least n ones, then scans the resulting word to
 *            find the required position
 * Range:     u8 n <= 78, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)             /* offset is 1, protect against 0 */
        return 0;

    /* initial word S0 = "0" */
    size_t cur_len = 1;
    uint8_t *cur = (uint8_t *)malloc(cur_len * sizeof(uint8_t));
    if (!cur) return 0;
    cur[0] = 0;

    while (1) {
        /* expand cur into next word */
        size_t new_cap = cur_len * 3 + 3;              /* safe upper bound */
        uint8_t *next = (uint8_t *)malloc(new_cap * sizeof(uint8_t));
        if (!next) { free(cur); return 0; }

        size_t new_len = 0;
        idx_t ones_in_next = 0;

        for (size_t i = 0; i < cur_len; ++i) {
            if (cur[i] == 0) {
                /* 0 → 01 */
                next[new_len++] = 0;
                next[new_len++] = 1;
                ++ones_in_next;
            } else {
                /* 1 → 000 */
                next[new_len++] = 0;
                next[new_len++] = 0;
                next[new_len++] = 0;
            }
        }

        free(cur);
        cur      = next;
        cur_len  = new_len;

        /* stop when the new word already contains enough 1’s */
        if (ones_in_next >= n)
            break;
    }

    /* scan the final word to find the n-th 1 */
    idx_t count = 0;
    for (idx_t i = 0; i < cur_len; ++i) {
        if (cur[i] == 1) {
            ++count;
            if (count == n) {
                seq_t result = (seq_t)(i + 1);   /* 1‑based index */
                /* clean up */
                free(cur);
                return result;
            }
        }
    }

    /* theoretically unreachable */
    free(cur);
    return 0;
}
