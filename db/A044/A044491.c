/*
 * A044491  Numbers n such that string 1,2 occurs in the base 6 representation
 *          of n but not of n+1.
 * https://oeis.org/A044491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044491:
 *  Numbers n such that the base‑6 representation of n contains the
 *  substring 1,2 but the base‑6 representation of n+1 does NOT.
 */
#include <stdbool.h>

/* Return true iff x (in base 6) contains the consecutive digits 1 followed by 2. */
static bool contains12(seq_t x)
{
    /* Scan from least significant digit; the pair "1 2" will be seen
     * as "2 1" when scanning backwards.  Therefore we look for previous
     * digit 2 followed by current digit 1. */
    int prev = -1;
    while (x > 0) {
        int d = (int)(x % 6);      /* current least significant digit */
        if (prev == 2 && d == 1)   /* found "12" in forward order */
            return true;
        prev = d;
        x /= 6;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for n = 0, but harmless */
        return 0;

    idx_t count = 0;
    for (idx_t cand = 1; ; ++cand) {
        if (contains12((seq_t)cand) && !contains12((seq_t)(cand + 1))) {
            ++count;
            if (count == n)
                return (seq_t)cand;
        }
    }
}
