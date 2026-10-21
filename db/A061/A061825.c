/*
 * A061825  Multiples of 7 containing only odd digits.
 * https://oeis.org/A061825
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the nth multiple of 7 that consists only of odd digits
 *            (1,3,5,7,9)
 * Range:     u8 n <= 7, u16 n <= 379, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* ODD digits that are allowed in the sequence */
static const int ODD[5] = {1, 3, 5, 7, 9};

/* Recursive helper that generates all odd‑digit numbers of a fixed length
   and counts those divisible by 7.  When the target position is reached
   the value is stored in *result and the recursion terminates early. */
static bool dfs(unsigned int len, unsigned int pos, seq_t cur,
                idx_t target, idx_t *cnt, seq_t *result)
{
    if (pos == len) {
        if (cur % 7 == 0) {
            (*cnt)++;
            if (*cnt == target) {
                *result = cur;
                return true;
            }
        }
        return false;
    }
    for (int i = 0; i < 5; ++i) {
        seq_t next = cur * 10 + ODD[i];
        if (dfs(len, pos + 1, next, target, cnt, result))
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t value = 0;
    /* Iterate over increasing number of digits until the nth valid term is found */
    for (unsigned int len = 1; ; ++len) {
        if (dfs(len, 0, 0, n, &count, &value))
            return value;
    }
}
