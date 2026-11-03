/*
 * A044624  Numbers n such that string 7,0 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Searches for the n-th integer that in base‑8 contains the
   substring 70 while its successor does not. Uses a simple
   linear scan. */

#include <stdbool.h>

/* Return true if the base‑8 representation of x contains the
   substring 70. */
static bool has_70(seq_t x)
{
    if (x == 0)
        return false;
    seq_t prev = 8;               /* sentinel value ≠ 0,7 */
    while (x > 0) {
        seq_t digit = x % 8;
        if (digit == 7 && prev == 0)
            return true;
        prev = digit;
        x /= 8;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined for offset < 1 */
    seq_t candidate = 56;          /* the first term of the sequence */
    idx_t found = 0;
    while (1) {
        if (has_70(candidate) && !has_70(candidate + 1)) {
            ++found;
            if (found == n)
                return candidate;
        }
        ++candidate;
    }
}
