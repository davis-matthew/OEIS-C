/*
 * A044255  Numbers n such that string 0,4 occurs in the base 9 representation
 *          of n but not of n-1.
 * https://oeis.org/A044255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th positive integer such that the base‑9 *
 *            representation of n contains the substring "04" while that of
 *            n‑1 does * not
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Return true iff the base‑9 representation of x contains the
 * consecutive digits 0 followed by 4 (most significant → least
 * significant). */
static bool contains04(seq_t x)
{
    if (x == 0) return false;
    int prev = -1;       /* previous (less significant) digit */
    while (x > 0) {
        int digit = (int)(x % 9);
        x /= 9;
        if (prev != -1 && digit == 0 && prev == 4) return true;
        prev = digit;
    }
    return false;
}

seq_t a(idx_t n)
{
    idx_t found = 0;
    idx_t candidate = 1;
    seq_t result = 0;

    while (1) {
        if (contains04((seq_t)candidate) &&
            !contains04((seq_t)(candidate - 1))) {
            ++found;
            if (found == n) {
                result = (seq_t)candidate;
                break;
            }
        }
        ++candidate;
    }
    return result;
}
