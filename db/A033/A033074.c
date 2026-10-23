/*
 * A033074  Numbers with the property that all pairs of consecutive base-9
 *          digits differ by 1.
 * https://oeis.org/A033074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumerates base‑9 numbers whose successive digits differ by 1. */
/* Returns the n‑th such number (1‑based index). */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 */

    /* ---- level 1: single‑digit numbers 1 … 8 ---- */
    size_t count = 0;               /* numbers produced so far     */
    size_t curr_len = 8;            /* length of current level     */
    seq_t *curr = (seq_t*)malloc(curr_len * sizeof(seq_t));
    if (!curr) return 0;            /* allocation failure */

    for (int d = 1; d <= 8; ++d) {
        count++;
        if (count == n) {
            seq_t val = (seq_t)d;
            free(curr);
            return val;
        }
        curr[count - 1] = (seq_t)d;
    }

    /* ---- breadth‑first expansion for longer numbers ---- */
    while (count < n) {
        size_t next_cap = curr_len * 2 + 1;   /* worst case size */
        seq_t *next = (seq_t*)malloc(next_cap * sizeof(seq_t));
        if (!next) break;                    /* safety; should not happen */

        size_t next_len = 0;

        for (size_t i = 0; i < curr_len; ++i) {
            seq_t num = curr[i];
            int last = (int)(num % 9);

            if (last > 0)          /* append digit (last-1) */
                next[next_len++] = num * 9 + (last - 1);

            if (last < 8)          /* append digit (last+1) */
                next[next_len++] = num * 9 + (last + 1);
        }

        free(curr);          /* old level no longer needed */
        curr = next;
        curr_len = next_len;

        for (size_t i = 0; i < curr_len && count < n; ++i) {
            count++;
            if (count == n) {
                seq_t result = curr[i];
                free(curr);
                return result;
            }
        }
    }

    /* ---- fallback (should not occur for valid input) ---- */
    if (curr) free(curr);
    return 0;
}
