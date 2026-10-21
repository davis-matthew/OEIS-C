/*
 * A060980  |First digit - second digit + third digit - fourth digit ...| =
 *          12.
 * https://oeis.org/A060980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth positive integer whose alternating sum of decimal digits
   (starting with the first digit as +) has absolute value 12. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n < 1 */

    idx_t found = 0;            /* how many valid terms have been found   */
    seq_t result = 0;           /* current candidate number               */

    /* iterate over positive integers until the nth matching one is found */
    for (seq_t num = 1;; ++num)
    {
        /* extract decimal digits of num (least‑significant first) */
        uint8_t digits[128];     /* enough for 128‑bit numbers (<= 39 digits) */
        int len = 0;
        seq_t tmp = num;

        do
        {
            digits[len++] = (uint8_t)(tmp % 10);
            tmp /= 10;
        } while (tmp != 0 && len < (int)sizeof(digits));

        /* compute alternating signed sum starting with the most significant digit */
        int sum = 0;
        int sign = 1;            /* + for the most significant digit */
        for (int i = len - 1; i >= 0; --i)
        {
            sum += sign * (int)digits[i];
            sign = -sign;
        }

        if (abs(sum) == 12)
        {
            ++found;
            result = num;
            if (found == n)
                return result;
        }
    }
}
