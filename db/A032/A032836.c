/*
 * A032836  Numbers whose set of base-12 digits is {3,4}.
 * https://oeis.org/A032836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the nth integer whose base‑12 representation
 * contains only the digits 3 and 4.  The offset is 1 (a(1)=3).  This
 * implementation maps the binary representation of (n-1) to a base‑12
 * number with digits 3 (for 0) and 4 (for 1).  It runs in O(log n)
 * time and uses only integer arithmetic. */
#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find the length of the base-12 number and the position within that
     * length group.  For each length L there are 2^L possibilities. */
    idx_t rem = n;
    idx_t len = 0;
    idx_t pow2 = 1;          /* 2^len, initially 2^0 = 1 */

    for (;;) {
        ++len;               /* increase length to L */
        pow2 <<= 1;          /* pow2 = 2^len */
        if (rem <= pow2)    /* required number is within this length group */
            break;
        rem -= pow2;        /* skip all numbers of this length */
    }

    /* Now rem-1 is the binary index within the group of length 'len'. */
    idx_t idx = rem - 1;    /* 0‑based binary index */
    seq_t value = 0;

    for (idx_t i = 0; i < len; ++i) {
        idx_t shift = len - 1 - i;                     /* most significant bit first */
        seq_t digit = ((idx >> shift) & 1) ? 4 : 3;    /* map bit to digit */
        value = value * 12 + digit;                    /* accumulate base‑12 value */
    }
    return value;
}
