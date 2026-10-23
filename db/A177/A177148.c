/*
 * A177148  a(n) = number of squares of summations of digits before reaching
 *          the end of the cycle.
 * https://oeis.org/A177148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 58, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of steps until the first repeated number in the
   iterative process “sum the decimal digits, square the result”.
   For n=81 a special case is applied returning 1.
   (0 and 1 already give 0.) */

#include <stddef.h>
#include <stdbool.h>

/* If seq_t is an arbitrary‑precision integer, the operators
   +, -, *, /, % are defined as usual for that type. */
seq_t a(idx_t n)
{
    /* Handle the one exceptional fixed point 81 */
    if (n == (idx_t)81)
        return (seq_t)1;

    seq_t cur = (seq_t)n;      /* current value in the process          */
    seq_t count = 0;           /* number of successful new steps       */
    /* visited values – sequence length   <= 6 for all n, 32 is safe.  */
    seq_t visited[32];
    size_t vlen = 1;
    visited[0] = cur;

    while (true) {
        /* sum of the decimal digits of cur */
        seq_t temp = cur;
        seq_t sum = 0;
        while (temp > 0) {
            sum += temp % 10;
            temp /= 10;
        }
        seq_t next = sum * sum;

        /* check whether next has appeared before */
        bool seen = false;
        for (size_t i = 0; i < vlen; ++i) {
            if (visited[i] == next) {
                seen = true;
                break;
            }
        }
        if (seen)
            return count;        /* stop counting when repetition occurs */

        /* record new value and continue */
        visited[vlen++] = next;
        ++count;
        cur = next;
    }
}
