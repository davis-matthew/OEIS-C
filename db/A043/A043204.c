/*
 * A043204  Numbers k such that 3 and 7 occur juxtaposed in the base-9
 *          representation of k but not of k-1.
 * https://oeis.org/A043204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th integer k such that the base‑9 representation of
 *            k contains the substring "37" or "73", while the base‑9
 *            representation of k−1 does not. The sequence is obtained by an
 *            exhaustive search (no formula or recurrence is known)
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses only integer arithmetic, operator % and /,
   and works for all allowed seq_t types (including arbitrary‑precision). */

#include <stdbool.h>

static bool has_adjacent_3_7(seq_t x)
{
    /* Return true iff the base‑9 digits of x contain 3 and 7
       next to each other (either 37 or 73). */
    const seq_t nine = (seq_t)9;
    if (x == (seq_t)0)
        return false;
    seq_t prev = x % nine;
    x /= nine;
    while (x > (seq_t)0) {
        seq_t cur = x % nine;
        if ((prev == (seq_t)3 && cur == (seq_t)7) ||
            (prev == (seq_t)7 && cur == (seq_t)3))
            return true;
        prev = cur;
        x /= nine;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Return the n‑th term of the sequence (offset 1).  For n == 0
       the result is undefined; we simply return 0. */
    if (n == (idx_t)0)
        return (seq_t)0;

    seq_t candidate = (seq_t)1;
    idx_t count = (idx_t)0;

    for (;;) {
        if (has_adjacent_3_7(candidate) &&
            !has_adjacent_3_7(candidate - (seq_t)1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
