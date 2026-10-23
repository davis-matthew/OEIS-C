/*
 * A044204  Numbers n such that string 2,1 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* sequence A044204: numbers n whose base‑8 representation contains
   the substring "21" while that of n-1 does not.  */
#include <stdint.h>
#include <stdbool.h>

/* Return true if v's base‑8 representation contains the pattern 2,1
   (most significant 2 followed by 1). */
static bool has21(seq_t v)
{
    /* For 0 return false – it has no digits. */
    if (v == 0)
        return false;

    /* digits[] holds the base‑8 digits in little‑endian order
       (least‑significant first).  Since seq_t is at most 128 bits,
       128/3 < 50, so 64 bytes is ample. */
    unsigned char digits[64] = {0};
    unsigned len = 0;

    while (v != 0) {
        digits[len++] = (unsigned char)(v % 8);
        v /= 8;
    }

    /* Look for pattern 1 (lower order) followed by 2 (higher order). */
    for (unsigned i = 0; i + 1 < len; ++i) {
        if (digits[i] == 1 && digits[i + 1] == 2)
            return true;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* The sequence starts at n = 1.  We simply scan up from 1,
       counting qualifying numbers. */
    seq_t candidate = 1;
    idx_t count = 0;

    for (;;) {
        if (has21(candidate) && !has21(candidate - 1)) {
            ++count;
            if (count == n)
                return candidate;
        }
        ++candidate;
    }
}
