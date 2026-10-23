/*
 * A044131  Numbers n such that string 4,5 occurs in the base 6 representation
 *          of n but not of n-1.
 * https://oeis.org/A044131
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n): the n-th positive integer whose base‑6 representation contains
 * the substring "45" but whose predecessor does not.
 * Computation: linear search with base‑6 digit extraction. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/* Return true if the base‑6 representation of x contains the
 * consecutive digits 4 followed by 5. */
static bool has45(idx_t x)
{
    /* Maximum 6‑digit length for 128‑bit numbers is < 64 */
    unsigned int digits[64];
    size_t len = 0;
    seq_t t = (seq_t)x;

    if (t == 0) {
        return false;               /* "0" has no 45 substring */
    }
    while (t > 0) {
        digits[len++] = (unsigned int)(t % 6);
        t /= 6;
    }
    /* Scan from most significant to least; digits[len-1] is MSB */
    for (size_t i = len; i-- > 1; ) {
        if (digits[i] == 4 && digits[i - 1] == 5)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for n < 1, but defined for safety */
        return (seq_t)0;

    idx_t count = 0;
    idx_t candidate = 1;

    while (count < n) {
        if (has45(candidate) && !has45(candidate - 1)) {
            ++count;
            if (count == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
    return (seq_t)0;       /* should never reach here */
}
