/*
 * A043997  Numbers k such that 0 and 2 occur juxtaposed in the base-10
 *          representation of k but not of k+1.
 * https://oeis.org/A043997
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: brute‑force search for k such that 0 and 2 occur juxtaposed
 * in the decimal representation of k but not in k+1.
 * a(1) = 20, a(2) = 102, ... as in OEIS A043997. */
#include <stdint.h>
#include <stdbool.h>

/* helper: checks whether an unsigned 64‑bit integer has the pair 02 or 20
 * in its decimal representation. */
static bool has_adjacent_02_or_20(uint64_t x)
{
    int prev = -1;                  /* previous (more significant) digit */
    while (x > 0) {
        int cur = x % 10;           /* current (least significant) digit */
        if ((prev == 0 && cur == 2) ||          /* 20 */
            (prev == 2 && cur == 0))            /* 02 */
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* returns the n‑th term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    if (n == 0)  /* the sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;

    idx_t found = 0;
    uint64_t num  = 1;              /* test integers starting from 1 */

    while (found < n) {
        if (has_adjacent_02_or_20(num) &&
            !has_adjacent_02_or_20(num + 1)) {
            found++;
            if (found == n)
                return (seq_t)num; /* cast to seq_t (assumes a constructor
                                     * or conversion from uint64_t) */
        }
        num++;
    }
    /* unreachable */
    return (seq_t)0;
}
