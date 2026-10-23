/*
 * A119353  Smallest value obtained by writing n in base b > 1 such that n in
 *          base b uses no digit b-1 and then interpreting this as a base b-1
 *          number.
 * https://oeis.org/A119353
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 317, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Detail: For each n, find the smallest base b>1 such that the base‑b
   representation of n contains no digit b-1, then interpret the same
   digit sequence as a number in base b-1.
   Return the resulting value. */
#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)   /* a(0) = 0 by definition */
        return (seq_t)0;

    /* maximal number of digits needed for idx_t values (≤ 64 for base 2) */
    uint16_t digits[64];

    for (idx_t b = 2; b <= n + 2; ++b)   /* n+2 always works */
    {
        /* Convert n to base b */
        seq_t tmp = (seq_t)n;
        size_t len = 0;
        do
        {
            digits[len++] = (uint16_t)(tmp % (seq_t)b);
            tmp /= (seq_t)b;
        } while (tmp > 0);

        /* Check that no digit equals b-1 */
        bool bad = false;
        seq_t maxdigit = (seq_t)(b - 1);
        for (size_t i = 0; i < len; ++i)
        {
            if ((seq_t)digits[i] == maxdigit)
            {
                bad = true;
                break;
            }
        }

        if (!bad)
        {
            /* Interpret the same digits in base (b-1) */
            seq_t val = 0;
            seq_t base_minus = (seq_t)(b - 1);
            for (size_t i = 0; i < len; ++i)
                val = val * base_minus + (seq_t)digits[len - 1 - i];
            return val;
        }
    }

    /* This point is unreachable: b = n+2 is guaranteed to succeed. */
    return (seq_t)0;
}
