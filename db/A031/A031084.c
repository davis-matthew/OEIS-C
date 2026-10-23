/*
 * A031084  Position of n-th 7 in A031076.
 * https://oeis.org/A031084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): position of the n-th 7 digit in the concatenation of
 * the base‑9 representations of the positive integers.  The function
 * scans the integers in order, converts each to base‑9, counts digits,
 * and records the cumulative position whenever the digit 7 is seen. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is >= 1 (offset 1) */
    seq_t  pos   = 0;   /* current position in the concatenated stream */
    idx_t  count = 0;   /* number of 7's seen so far */
    idx_t  k     = 1;   /* current integer to convert to base‑9 */
    const uint8_t base = 9;

    while (count < n)
    {
        /* Convert k to base‑9 digits (least significant first). */
        idx_t tmp   = k;
        uint8_t digits[32];   /* enough for k up to 64 bits in base 9 */
        idx_t  len   = 0;
        do
        {
            digits[len++] = tmp % base;
            tmp /= base;
        } while (tmp > 0);

        /* Process digits from most significant to least. */
        for (idx_t i = len; i > 0; --i)
        {
            pos++;                     /* advance position */
            if (digits[i - 1] == 7)    /* found a 7 digit */
            {
                count++;
                if (count == n)        /* reached the desired 7 */
                    return pos;
            }
        }
        k++;   /* move to next integer */
    }
    /* The loop always returns, but keep a sentinel. */
    return pos;
}
