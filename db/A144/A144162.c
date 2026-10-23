/*
 * A144162  Numbers with at least two digits in which all digits except the
 *          rightmost are 0 or 5 and the rightmost is neither 0 nor 5.
 * https://oeis.org/A144162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A144162: Numbers whose all but the last digit are 0 or 5,
   and the last digit is neither 0 nor 5.  The terms are sorted
   in ascending order.  The n‑th term is obtained by
   determining the length L of the number, the binary pattern
   of the intermediate 0/5 digits, and the final digit. */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Find the length L (>=2) such that the cumulative count
       of terms up to length L-1 is less than n and the count up
       to L is at least n.  T(L) = 2^(L-2) * 8. */
    idx_t L = 2;
    idx_t cumulative = 0;
    while (1)
    {
        idx_t count = (((idx_t)1) << (L - 2)) * (idx_t)8;
        if (cumulative + count >= n)
            break;
        cumulative += count;
        ++L;
    }

    /* Convert index to 0-based offset within the block of length L. */
    idx_t offset = n - cumulative - (idx_t)1;     /* 0‑based */
    idx_t combo  = offset / (idx_t)8;             /* pattern for 0/5 digits */
    idx_t last   = offset % (idx_t)8;             /* index into last_digit array */

    /* Base number with '5' in the most significant position. */
    seq_t num = (seq_t)5 * pow10((idx_t)L - 1);

    /* Set the intermediate 0/5 digits according to combo bits.
       Most significant variable digit corresponds to the highest bit. */
    for (idx_t p = 1; p <= L - 2; ++p)
    {
        idx_t shift = (L - 2) - p;
        if ((combo >> shift) & (idx_t)1)
            num += (seq_t)5 * pow10((idx_t)L - 1 - p);
    }

    /* Append the final digit (never 0 or 5). */
    static const seq_t last_digits[8] = {1, 2, 3, 4, 6, 7, 8, 9};
    num += last_digits[last];

    return num;
}
