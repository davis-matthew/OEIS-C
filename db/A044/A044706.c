/*
 * A044706  Numbers n such that string 8,2 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044706
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n‑th integer whose base‑9 representation contains the
 *            substring 82 but the base‑9 representation of n+1 does NOT
 *            contain 82. The function searches sequentially for the required
 *            numbers
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Check whether seq_t x satisfies the definition:
   base‑9 representation of x contains “82” and that of x+1 does not. */
static bool has_pattern(seq_t x)
{
    /* Store base‑9 digits least significant first. */
    unsigned char d0[128];
    unsigned char d1[128];
    int len0 = 0, len1;

    /* Convert x to base‑9. */
    seq_t t = x;
    while (t != 0) {
        d0[len0++] = (unsigned char)(t % 9);
        t = t / 9;
    }
    if (len0 == 0) {          /* x == 0, never occurs here, but be safe   */
        d0[0] = 0;
        len0 = 1;
    }

    /* Does x have at least one “82”? */
    bool have = false;
    for (int i = 1; i < len0; ++i) {
        if (d0[i] == 8 && d0[i - 1] == 2) {
            have = true;
            break;
        }
    }
    if (!have)
        return false;

    /* Compute digits of x+1.
       Start with a copy of d0 and perform addition in base‑9. */
    for (int i = 0; i < len0; ++i)
        d1[i] = d0[i];
    len1 = len0;

    int carry = 1;
    int i = 0;
    while (carry && i < len1) {
        int sum = d1[i] + carry;    /* d1[i] is <= 8, carry is 1 */
        if (sum >= 9) {
            d1[i] = (unsigned char)(sum - 9);
            carry = 1;
        } else {
            d1[i] = (unsigned char)sum;
            carry = 0;
        }
        ++i;
    }
    if (carry) {
        d1[len1++] = (unsigned char)carry;   /* carry is 1 */
    }

    /* Does x+1 contain “82”?  If so, x does NOT satisfy the definition. */
    for (int j = 1; j < len1; ++j) {
        if (d1[j] == 8 && d1[j - 1] == 2)
            return false;
    }
    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    seq_t candidate = 1;
    idx_t count = 0;

    while (count < n) {
        if (has_pattern(candidate)) {
            ++count;
            if (count == n)
                return candidate;
        }
        candidate += 1;  /* equivalent to ++candidate, works for all seq_t */
    }
    /* Unreachable, but keep the compiler happy. */
    return candidate;
}
