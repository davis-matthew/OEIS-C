/*
 * A044284  Numbers n such that string 3,6 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 890, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 984 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers n such that the base‑9 representation of n contains
   the substring "36" but n‑1 does not.  The function searches integers
   sequentially until it has found the n-th such number. */
#include <stdbool.h>
#include <stddef.h>

/* true if x in base 9 has digits ...36... (most significant 3 then 6) */
static bool contains_36(seq_t x)
{
    if (x == 0)
        return false;

    /* store the base‑9 digits least significant first */
    uint8_t digits[64];
    size_t len = 0;

    while (x > 0) {
        digits[len++] = (uint8_t)(x % 9);
        x = x / 9;
    }

    /* scan for a '3' followed immediately by a '6' (more significant) */
    for (size_t i = 1; i < len; ++i)
        if (digits[i] == 3 && digits[i - 1] == 6)
            return true;
    return false;
}

seq_t a(idx_t n)
{
    seq_t curr = 1;      /* start checking from 1 */
    idx_t found = 0;     /* how many terms found so far */

    while (1) {
        if (contains_36(curr) && !contains_36(curr - 1)) {
            ++found;
            if (found == n)
                return curr;
        }
        ++curr;
    }
}
