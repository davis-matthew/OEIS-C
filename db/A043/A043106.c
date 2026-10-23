/*
 * A043106  Numbers k such that 0 and 1 occur juxtaposed in the base-5
 *          representation of k but not of k-1.
 * https://oeis.org/A043106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the smallest k such that * – the base‑5 representation
 *            of k contains an adjacent pair of digits 0 and 1 * – the base‑5
 *            representation of k-1 does **not** contain such a pair * This
 *            function searches sequentially, so it works for any n and in any
 *            * calling order. No memoisation or dynamic...
 * Range:     u8 n <= 17, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true if v (in base 5) has an adjacent pair of digits 0 and 1. */
static bool has_adjacent_01(seq_t v)
{
    /* Special case: the number 0 has no digits, thus cannot contain the pattern. */
    if (v == 0) {
        return false;
    }
    int prev = (int)(v % 5);   /* least significant digit */
    v /= 5;
    while (v > 0) {
        int cur = (int)(v % 5);
        if ((prev == 0 && cur == 1) || (prev == 1 && cur == 0)) {
            return true;
        }
        prev = cur;
        v /= 5;
    }
    return false;
}

/* Main sequence routine.  */
seq_t a(idx_t n)
{
    /* n is guaranteed >= 1 for OEIS usage. */
    seq_t k = 0;                /* candidate number   */
    idx_t count = 0;            /* how many terms found so far */

    while (count < n) {
        ++k;
        if (has_adjacent_01(k) && !has_adjacent_01(k - 1)) {
            ++count;
        }
    }
    return k;
}
