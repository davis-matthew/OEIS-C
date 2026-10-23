/*
 * A125098  Values occurring in A123895.
 * https://oeis.org/A125098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 202, u32 n <= 876, u64 n <= 876,
 *            u128 n <= 876, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose decimal digits form a restricted growth
   string (each digit is 0, has appeared before, or is one more than
   the current maximum). Numbers are produced in increasing order
   (length ascending, then lexicographic within the same length). */
#include <stdbool.h>

static bool dfs_gen(int len, int pos, int max_digit, seq_t cur,
                    idx_t *counter, idx_t n, seq_t *res)
{
    if (pos == len) {
        if (*counter == n) {
            *res = cur;
            return true;
        }
        (*counter)++;
        return false;
    }
    for (int d = 0; d <= max_digit + 1; ++d) {
        seq_t nxt = cur * 10 + d;
        int nxt_max = max_digit;
        if (d > max_digit) nxt_max = d;
        if (dfs_gen(len, pos + 1, nxt_max, nxt, counter, n, res))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t counter = 0;
    /* Length 1 numbers: 0, 1 */
    seq_t val = 0;
    if (counter == n) return val;
    counter++;
    val = 1;
    if (counter == n) return val;
    counter++;

    /* Length ≥ 2 numbers */
    for (int len = 2; ; ++len) {
        seq_t res = 0;
        if (dfs_gen(len, 1, 1, 1, &counter, n, &res))
            return res;
    }
}
