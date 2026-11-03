/*
 * A044113  Numbers n such that string 1,5 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044113
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: partial, pass, 44 terms through n = 44 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A044113: numbers n such that the base‑6 representation of n
 * contains the substring “15” but the base‑6 representation of n‑1 does
 * not.  The function returns the n‑th such integer (OEIS offset 1).
 */

#include <stdbool.h>

/* Check whether the base‑6 representation of x contains the substring 15 */
static bool base6_has_15(seq_t x)
{
    /* Zero has no digits matching “15” */
    if (x == 0)
        return false;

    /* Store base‑6 digits in reverse order: least significant first. */
    uint8_t digits[64];  /* 6^64 > 2^256, enough for uint128 and beyond */
    idx_t len = 0;
    seq_t tmp = x;
    while (tmp != 0) {
        digits[len++] = (uint8_t)(tmp % 6);
        tmp /= 6;
    }

    /* Scan from most significant digit to least (reverse the array) */
    for (idx_t i = len; i > 1; --i) {
        if (digits[i - 1] == 1 && digits[i - 2] == 5)
            return true;
    }
    return false;
}

/*
 * a(n):
 *   n ≥ 1
 *   Return the n-th integer whose base‑6 representation contains the
 *   substring “15” but the representation of the predecessor does not.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0; /* undefined for n < 1, but harmless */

    idx_t count = 0;
    seq_t candidate = 1;

    while (true) {
        if (base6_has_15(candidate) && !base6_has_15(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
