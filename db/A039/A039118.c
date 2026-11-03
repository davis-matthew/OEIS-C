/*
 * A039118  Numbers whose base-10 representation has the same number of 0's
 *          and 7's.
 * https://oeis.org/A039118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number at index n that in base 10 has the same count of
 *            digits 0 and 7 (offset 1: a(1)=1)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 70 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper to test a number – treat as unsigned integer of sufficiently large range.
   It is safe for idx_t values used in the loop. */
static bool equal_zero_seven(idx_t x)
{
    int zeros = 0, sevens = 0;
    while (x > 0) {
        int d = x % 10;
        if (d == 0) zeros++;
        else if (d == 7) sevens++;
        x /= 10;
    }
    return zeros == sevens;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;               /* start from 1, a(1) = 1 */

    while (count < n) {
        if (equal_zero_seven(candidate)) {
            count++;
            if (count == n) return (seq_t)candidate;
        }
        candidate++;
    }
    /* unreachable – loop terminates when count reaches n */
    return (seq_t)0;
}
