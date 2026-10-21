/*
 * A224166  Number of halving and tripling steps to reach the last number of
 *          the cycle in the Collatz (3x+1) problem for the negative integers
 *          (the initial term is counted).
 * https://oeis.org/A224166
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct terms in the trajectory of -n until
 *            the first repeated value (i.e., the cycle length of -n). The
 *            computation is performed on the positive integer y = n with the
 *            recurrence y even : y = y / 2 y odd : y = 3*y - 1 which is
 *            equivalent to the Collatz mapping for negative inte...
 * Range:     u8 n <= 16, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_PATH 8192          /* safe upper bound for path length */

static seq_t visited[MAX_PATH];

seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* offset starts at 1, but guard against 0 */

    seq_t y = (seq_t)n;        /* positive representative of |-n| */
    size_t len = 0;

    while (1) {
        /* check if y has appeared before */
        int seen = 0;
        for (size_t i = 0; i < len; ++i) {
            if (visited[i] == y) { seen = 1; break; }
        }
        if (seen) break;          /* first repeat found */

        visited[len++] = y;       /* store new value */

        /* next term of the trajectory */
        if ((y & 1) == 0)
            y /= 2;
        else
            y = y * 3 - 1;
    }

    return (seq_t)len;           /* number of distinct terms visited */
}
