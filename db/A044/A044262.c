/*
 * A044262  Numbers n such that string 1,2 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044262: numbers n whose base‑9 representation contains the
   substring "12" but n‑1 does not.  Computed by brute‑force search. */
#include <stdbool.h>

static bool has12_base9(idx_t x)
{
    int prev = -1;
    /* iterate digits from least‑significant to most‑significant.
       In this order a forward substring 1,2 is seen when we
       encounter a 1 after having seen a 2. */
    while (x > 0) {
        int d = x % 9;
        if (prev == 2 && d == 1) return true;
        prev = d;
        x /= 9;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    idx_t candidate = 1;   /* search starts from 1 */

    while (1) {
        if (has12_base9(candidate) && !has12_base9(candidate - 1)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
