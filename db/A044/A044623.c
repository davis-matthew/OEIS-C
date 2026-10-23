/*
 * A044623  Numbers n such that string 6,7 occurs in the base 8 representation
 *          of n but not of n+1.
 * https://oeis.org/A044623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044623:
   a(n) = the n-th integer n such that the base‑8 representation
   of n contains the substring "67" but the base‑8 representation
   of n+1 does not. */
/* The computation is performed by a straightforward test
   of each candidate integer; the first n satisfying the
   condition are generated on demand. */

#include <stdbool.h>
#include <stdint.h>

/* Helper: test whether the given integer satisfies the
   property for sequence A044623. */
static bool has_property(seq_t x)
{
    /* Convert x to base‑8 digits (most significant first). */
    unsigned char buf[128];   /* maximum digits for 128‑bit numbers is < 40 */
    int len = 0;
    seq_t tmp = x;

    if (tmp == 0)  /* zero never satisfies the property */
        return false;

    while (tmp > 0) {
        buf[len++] = '0' + (int)(tmp % 8);
        tmp /= 8;
    }
    /* Reverse to get most significant digit first. */
    for (int i = 0; i < len / 2; ++i) {
        unsigned char c = buf[i];
        buf[i] = buf[len - 1 - i];
        buf[len - 1 - i] = c;
    }

    /* Count trailing 7's. */
    int trailing = 0;
    while (trailing < len && buf[len - 1 - trailing] == '7')
        ++trailing;
    if (trailing == 0)                 /* no "67" substring ending with 7  */
        return false;
    if (len <= trailing)               /* number consists only of trailing 7's */
        return false;
    if (buf[len - trailing - 1] != '6')
        return false;                  /* must have a leading 6 before the run */

    /* If there is a digit left of the leading 6, it must not be 6
       (otherwise adding 1 would create a new "67" in n+1). */
    if (len > trailing + 1 && buf[len - trailing - 2] == '6')
        return false;

    /* Check the prefix (digits before the leading 6) does not contain
       any "67" substring. */
    for (int i = 0; i <= len - trailing - 2; ++i)
        if (buf[i] == '6' && buf[i + 1] == '7')
            return false;

    return true;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)  /* numberings start at 1; return 0 for safety */
        return 0;

    seq_t candidate = 0;
    idx_t count = 0;

    while (count < n) {
        ++candidate;
        if (has_property(candidate))
            ++count;
    }
    return candidate;
}
