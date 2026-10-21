/*
 * A228007  The largest n-digit number whose first k digits are divisible by
 *          k^2 for k = 1..n.
 * https://oeis.org/A228007
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full
 * Formula:   Search for the largest n‑digit number whose first k digits are
 *            divisible by k^2 for all k = 1..n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Depth‑first search helper.
   current is the integer formed by the first pos‑1 digits.
   pos   is the next position to fill (1‑based).
   found points to a flag set when a complete n‑digit number is found.      */
static seq_t dfs(idx_t n, idx_t pos, seq_t current, bool *found)
{
    if (*found) return 0;                 /* already found a solution */

    if (pos == 1) {                       /* first digit cannot be 0 */
        for (int d = 9; d >= 1; --d) {
            seq_t new_cur = current * 10 + (seq_t)d;
            seq_t mod = (seq_t)pos * (seq_t)pos;
            if (new_cur % mod != 0) continue;
            if (n == 1) { *found = true; return new_cur; }
            seq_t res = dfs(n, pos + 1, new_cur, found);
            if (*found) return res;
        }
    } else {
        for (int d = 9; d >= 0; --d) {
            seq_t new_cur = current * 10 + (seq_t)d;
            seq_t mod = (seq_t)pos * (seq_t)pos;
            if (new_cur % mod != 0) continue;
            if (pos == n) { *found = true; return new_cur; }
            seq_t res = dfs(n, pos + 1, new_cur, found);
            if (*found) return res;
        }
    }
    return 0;                              /* no digit works for this pos */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    bool found = false;
    seq_t result = dfs(n, 1, 0, &found);
    return result;                         /* 0 if no n‑digit number exists */
}
