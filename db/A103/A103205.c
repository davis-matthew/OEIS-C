/*
 * A103205  Write numbers in decimal under each other, then read diagonals in
 *          upward direction.
 * https://oeis.org/A103205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = read the digits of the natural numbers n, n+1, … along
 *            the upward‑diagonal i − j = n (right‑aligned decimal). The
 *            result is the concatenation of these digits from the largest i
 *            to the smallest
 * Range:     u8 n <= 197, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   count_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_digits.h"

seq_t a(idx_t n)
{
    /* store the digits of the diagonal, at most 64 needed */
    seq_t digs[64];
    int cnt = 0;

    for (idx_t i = n; ; ++i) {
        idx_t j = i - n;                     /* column index (0 = units) */
        int  d = count_digits(i);            /* number of decimal digits of i */
        if (j >= (idx_t)d)                   /* no more numbers on this diagonal */
            break;

        /* extract the j‑th digit from the right of i */
        seq_t t = i;
        for (idx_t k = 0; k < j; ++k)
            t /= 10;
        seq_t digit = t % 10;

        digs[cnt++] = digit;
    }

    /* build the number from highest i to lowest */
    seq_t result = 0;
    for (int k = cnt - 1; k >= 0; --k)
        result = result * 10 + digs[k];

    return result;
}
