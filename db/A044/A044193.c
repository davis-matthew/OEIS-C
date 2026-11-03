/*
 * A044193  Numbers n such that string 0,6 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044193: nth integer whose octal representation contains the
 * substring "06" but the predecessor does not. The function searches
 * sequentially, avoiding a hard-coded table. */
#include <stdbool.h>

static bool has_06(seq_t x)
{
    seq_t prev = 8;               /* sentinel that cannot be a digit */
    while (x > 0) {
        seq_t d = x % 8;
        if (prev == 6 && d == 0) return true;   /* seen "...0 6" */
        prev = d;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* not part of the sequence */
    idx_t count = 0;

    for (idx_t m = 1; ; ++m) {
        seq_t cur  = (seq_t)m;
        if (has_06(cur)) {
            seq_t prev = (seq_t)(m - 1);
            if (!has_06(prev)) {
                ++count;
                if (count == n) return cur;
            }
        }
    }
}
