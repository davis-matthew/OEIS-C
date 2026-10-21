/*
 * A236366  a(n) is the concatenation of the numbers k, 2 <= k <= 9, such that
 *          the base-k representation of n is a palindrome; a(n) = 0 if there
 *          is no such base.
 * https://oeis.org/A236366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = concatenation of bases k (2 ≤ k ≤ 9) for which the base‑k
 *            representation of n is a palindrome; 0 if none
 * Range:     u8 none, u16 none, u32 n <= 5000, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    seq_t result = 0;                 /* concatenated qualifying bases */

    uint8_t digits[70];               /* stores base‑k digits (least significant first) */

    for (idx_t base = 2; base <= 9; ++base)
    {
        idx_t tmp = n;
        idx_t len = 0;

        /* Extract digits of n in base ‘base’. */
        while (tmp > 0)
        {
            digits[len++] = (uint8_t)(tmp % base);
            tmp /= base;
        }

        /* Determine if the digit sequence is a palindrome. */
        bool pal = true;
        for (idx_t i = 0; i < len / 2; ++i)
        {
            if (digits[i] != digits[len - 1 - i])
            {
                pal = false;
                break;
            }
        }

        if (pal)
        {
            result = result * 10 + (seq_t)base;
        }
    }

    return result;    /* 0 if no base produced a palindrome */
}
