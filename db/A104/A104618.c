/*
 * A104618  Write the natural numbers in base 15 in a triangle with k digits
 *          in the k-th row, as shown below. Sequence gives the leading
 *          diagonal.
 * https://oeis.org/A104618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Validated: partial, pass, 99 terms through n = 99 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the digit that appears at position n(n+1)/2 in the stream obtained
   by concatenating the base‑15 representations of the natural numbers
   (starting with 1). */

#include <stdint.h>

/* Find the digit at a given position in the base‑15 stream */
static seq_t digit_at_pos(seq_t pos) {
    idx_t cur = 1;           /* current natural number */
    seq_t cur_pos = 1;       /* starting position of cur in the stream */

    while (1) {
        /* Determine the number of base‑15 digits of cur */
        seq_t len = 0;
        idx_t t = cur;
        while (t > 0) {
            len++;
            t /= 15;
        }

        /* Does the requested position lie within cur? */
        if (pos <= cur_pos + len - 1) {
            seq_t offset = pos - cur_pos;   /* 0‑based offset inside cur */

            /* Extract the decimal digits of cur in base 15 in reverse order.
               The most significant digit will be at dig[d-1]. */
            uint8_t dig[32];
            t = cur;
            int d = 0;
            while (t > 0) {
                dig[d++] = t % 15;
                t /= 15;
            }

            /* Return the digit at the given offset from the most significant digit. */
            return dig[d - 1 - (int)offset];
        }

        /* Skip this number and move to the next. */
        cur_pos += len;
        cur++;
    }
}

/* External linkage function required by the OEIS build */
seq_t a(idx_t n) {
    /* Position of the last digit of row n */
    seq_t pos = ((seq_t)n * ((seq_t)n + 1)) / 2;
    return digit_at_pos(pos);
}
