/*
 * A043459  Numbers having three 1's in base 9.
 * https://oeis.org/A043459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043459: numbers having three 1's in base 9 */
#include <stdbool.h>

/* Forward declaration of the recursive generator */
static bool generate(idx_t len, idx_t pos, idx_t ones_left,
                     seq_t cur, idx_t *cnt, idx_t target, seq_t *res);

/* Return the n-th term (1‑based). */
seq_t a(idx_t n) {
    idx_t target = n;
    idx_t cnt = 0;
    seq_t res = 0;
    for (idx_t len = 3; ; ++len) {
        if (generate(len, 0, 3, (seq_t)0, &cnt, target, &res))
            return res;
    }
}

/* Recursive lexicographic generator.
 * cur holds the value built so far.
 * returns true when the target term has been found. */
static bool generate(idx_t len, idx_t pos, idx_t ones_left,
                     seq_t cur, idx_t *cnt, idx_t target, seq_t *res)
{
    if (pos == len) {
        if (ones_left == 0) {
            (*cnt)++;
            if (*cnt == target) {
                *res = cur;
                return true;
            }
        }
        return false;
    }

    idx_t d_start = (pos == 0) ? 1 : 0;
    for (idx_t d = d_start; d <= 8; ++d) {
        if (d == 1 && ones_left == 0) /* would exceed three 1's */
            continue;

        idx_t new_ones = ones_left;
        if (d == 1)
            --new_ones;

        idx_t remaining = len - pos - 1;
        if (new_ones > remaining)
            continue; /* cannot fit the remaining 1's */

        seq_t new_cur = cur * (seq_t)9 + (seq_t)d;
        if (generate(len, pos + 1, new_ones, new_cur, cnt, target, res))
            return true;
    }
    return false;
}
