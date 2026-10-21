/*
 * A238986  Write the decimal digits of n and take the absolute differences of
 *          the digits, repeating until a single digit remains. Then sum all
 *          digits of each level of the resulting triangle. If the sum is
 *          greater than 9, repeat the entire process until result is between
 *          0 and 9, which is a(n).
 * https://oeis.org/A238986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   iterative pyramidalization sum of n’s decimal digits until a
 *            single digit 0–9 is obtained
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert the index to a seq_t value (the sequence operates on decimal digits). */
    seq_t curr = (seq_t)n;                /* initial value to be processed   */
    while (1)
    {
        seq_t total = 0;                  /* sum of all digits in all levels */
        unsigned int digits[64];          /* buffer for the digit triangle    */
        size_t len = 0;

        /* Extract decimal digits of curr (least‑significant first). */
        if (curr == 0)
        {
            digits[0] = 0;
            len = 1;
        }
        else
        {
            seq_t tmp = curr;
            while (tmp > 0)
            {
                digits[len++] = (unsigned int)(tmp % 10);
                tmp /= 10;
            }
        }

        /* Build the pyramidal triangle, summing the digits at each level. */
        size_t cur_len = len;
        while (1)
        {
            for (size_t i = 0; i < cur_len; ++i)
                total += digits[i];

            if (cur_len == 1)
                break;   /* finished one triangle */

            /* Compute the next level by taking absolute differences of adjacent digits. */
            for (size_t i = 0; i < cur_len - 1; ++i)
            {
                unsigned int diff = digits[i] > digits[i + 1] ?
                                    digits[i] - digits[i + 1] :
                                    digits[i + 1] - digits[i];
                digits[i] = diff;
            }
            cur_len--;              /* proceed to the new, shorter level   */
        }

        /* If the accumulated sum is already a single digit, we are done. */
        if (total <= (seq_t)9)
            return total;

        /* Otherwise continue the process with the new value. */
        curr = total;
    }
}
