/*
 * A029581  Numbers in which all digits are composite.
 * https://oeis.org/A029581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 660, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A029581: numbers whose all digits are composite (4,6,8,9).
   For a given index n (>=1) we compute a(n) by interpreting n in a
   “zero‑free” base‑4 representation:
         digits 1,2,3,4  →  4,6,8,9
   This yields exactly the sequence listed in OEIS.                */
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t nn = n;          /* copy of the index since we modify it */
    seq_t result = 0;      /* the number we build */
    seq_t mult = 1;        /* 10^position */

    while (nn > 0) {       /* process least significant digit first */
        idx_t digit = nn % 4;
        if (digit == 0) {  /* zero turned into a 4 with a borrow */
            digit = 4;
            nn = nn / 4 - 1;
        } else {
            nn = nn / 4;
        }

        /* map the base‑4 digit to the composite digit 4,6,8,9 */
        seq_t comp = (seq_t)(digit == 4 ? 9 : 4 + (digit - 1) * 2);

        result += comp * mult;
        mult *= 10;        /* next power of 10 */
    }
    return result;
}
