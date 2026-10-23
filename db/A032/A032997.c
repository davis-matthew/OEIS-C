/*
 * A032997  If d,e are consecutive digits of n in base 9, then |d-e|>=5.
 * https://oeis.org/A032997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A032997: numbers whose base‑9 digits have |d-e| ≥ 5 */

/* Return the n-th positive integer (n ≥ 1) such that in base 9,
   every pair of consecutive digits differ by at least 5.  This
   implementation generates the numbers in increasing order by
   building them digit by digit.  It is O(N) where N is the required
   index and works for any integer types seq_t/idx_t supplied by
   the build. */

#include <stdbool.h>

/* Depth‑first search for a given number of remaining digits.
   *rem         – how many digits still need to be appended
   *cur         – number built so far
   prev          – last digit appended (used to check adjacency)
   *cnt          – current count of generated numbers
   n             – target index
   *result       – where to store the found number
   Returns true if the target was found. */
static bool dfs(uint64_t rem, seq_t cur, int prev, idx_t *cnt,
                idx_t n, seq_t *result)
{
    if (rem == 0) {
        (*cnt)++;                     /* a new valid number is formed */
        if (*cnt == n) {
            *result = cur;
            return true;
        }
        return false;
    }
    for (int d = 0; d <= 8; ++d) {
        int diff = prev - d;
        if (diff < 0) diff = -diff;
        if (diff < 5) continue;        /* adjacency not satisfied */
        seq_t next = cur * (seq_t)9 + (seq_t)d;
        if (dfs(rem - 1, next, d, cnt, n, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    seq_t value = 0;

    /* Length 1: digits 1 … 8 */
    for (int first = 1; first <= 8; ++first) {
        value = (seq_t)first;
        cnt++;
        if (cnt == n) return value;
    }

    /* Length ≥ 2 : generate numbers by DFS */
    for (uint64_t len = 2; ; ++len) {
        for (int first = 1; first <= 8; ++first) {
            value = (seq_t)first;
            if (dfs(len - 1, value, first, &cnt, n, &value))
                return value;
        }
    }
}
