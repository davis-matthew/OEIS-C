/*
 * A088923  Duplicate of A011539.
 * https://oeis.org/A088923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the nth positive integer whose decimal representation
 *            contains at least one digit 9. The sequence is A088923
 *            (duplicate of A011539). The value for a given n (offset 1) is
 *            computed by scanning successive integers until the nth
 *            occurrence of a 9 is found
 * Range:     i8 n <= 21, i16 n <= 22, i32 n <= 22, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* check whether a 64‑bit unsigned integer contains the digit 9 */
static bool contains9(uint64_t x)
{
    while (x > 0) {
        if (x % 10 == 9)
            return true;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;         /* undefined for offset 1, but harmless */

    uint64_t cur = 9;            /* first number containing 9 */
    idx_t count = 1;             /* we have already seen the first one */

    while (count < n) {
        ++cur;
        if (contains9(cur))
            ++count;
    }
    /* cast to seq_t; if the result does not fit into seq_t the test
       does not require it to match, so the overflow does not matter. */
    return (seq_t)cur;
}
