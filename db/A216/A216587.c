/*
 * A216587  Preimage of n for A216556 (i.e., concatenation of digits of a(n),
 *          each increased by 1, yields n), -1 if there is none.
 * https://oeis.org/A216587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, easy
 * Formula:   not stated
 * Range:     i8 n <= 74, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A216587: Preimage of n for A216556.
   Definition:  a(n) is the unique integer m such that
   concatenating (digit+1) for each digit of m yields the decimal
   representation of n; return -1 if no such m exists.
   Algorithm:   parse the decimal digits of n from most significant to
   least significant.  A digit '1' followed by '0' must be taken as the
   two‑digit pattern "10" (digit 9), otherwise a single digit '1' is
   interpreted as digit 0.  Any other digit d in 2…9 becomes d-1.  A
   lone '0' or a leading '0' in the generated m (unless m consists
   solely of that zero) makes the preimage impossible.
*/

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Build the decimal digits of n, most significant first. */
    uint8_t rev[32];
    size_t revcnt = 0;
    idx_t tmp = n;

    if (tmp == 0) {
        rev[0] = 0;
        revcnt = 1;
    } else {
        while (tmp > 0) {
            rev[revcnt++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        }
    }

    uint8_t digits[32];
    for (size_t i = 0; i < revcnt; ++i)
        digits[i] = rev[revcnt - 1 - i];   /* now digits[0] is most significant */

    /* Parse digits to reconstruct m's digits, stored as m_digits. */
    uint8_t m_digits[32];
    size_t mcnt = 0;
    size_t pos = 0;

    while (pos < revcnt) {
        uint8_t cur = digits[pos];

        /* Digit '0' cannot start a valid substring. */
        if (cur == 0)
            return -1;

        if (cur == 1) {
            /* Lookahead for the pattern "10". */
            if (pos + 1 < revcnt && digits[pos + 1] == 0) {
                m_digits[mcnt++] = 9;          /* digit 9 */
                pos += 2;
            } else {
                m_digits[mcnt++] = 0;          /* digit 0 */
                pos += 1;
            }
        } else { /* cur in 2..9 */
            m_digits[mcnt++] = cur - 1;
            pos += 1;
        }
    }

    /* No digits reconstructed => impossible. */
    if (mcnt == 0) return -1;

    /* Leading zero invalid unless m consists of a single zero. */
    if (mcnt > 1 && m_digits[0] == 0)
        return -1;

    /* Convert digit array to an integer of type seq_t. */
    seq_t res = 0;
    for (size_t i = 0; i < mcnt; ++i)
        res = res * 10 + m_digits[i];

    return res;
}
