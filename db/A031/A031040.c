/*
 * A031040  Position of n-th 4 in A031035.
 * https://oeis.org/A031040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Position of the n-th occurrence of the digit 4 in the
   concatenation of natural numbers written in base‑8 (octal).
   Indexing starts at 1 (so a(1) = 4). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* current position in the infinite digit string (1‑based) */
    seq_t pos = 1;
    /* number of 4's seen so far */
    idx_t seen = 0;

    /* iterate over successive positive integers in decimal */
    for (idx_t m = 1; ; ++m) {
        /* convert m to its base‑8 digits (least significant first) */
        uint8_t digits[32];   /* 32 > floor(log_8(2^64)) + 1 */
        int len = 0;
        idx_t tmp = m;
        do {
            digits[len++] = tmp % 8;
            tmp /= 8;
        } while (tmp);

        /* scan digits most significant first */
        for (int i = len - 1; i >= 0; --i) {
            if (digits[i] == 4) {          /* a 4 has been found */
                ++seen;
                if (seen == n)
                    return pos;           /* return its 1‑based position */
            }
            ++pos;                         /* move to next position */
        }
    }
}
