/*
 * A057888  In Japanese: 1-digit numbers in a-i-u-e-o order, then 2-digit
 *          numbers in a-i-u-e-o order, etc.
 * https://oeis.org/A057888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, nonn, word
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithmic description:  A057888 lists positive integers sorted by
 * the Japanese vowel order of their decimal digits.
 * For single‑digit numbers the order is:
 *   1,9,5,3,7,2,8,4,0,6
 * For length ≥ 2, the leading digit uses the same order but with 1
 * moved to the fourth position, and the remaining digits are ordered
 * as 0,1,9,5,3,7,2,8,4,6.
 * The function computes a(n) by determining the length of the number
 * and converting the offset within that length block into the
 * corresponding digits.  It works for any seq_t that is an unsigned
 * integer (up to 128 bits or arbitrary precision).  No external
 * libraries are required.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Digit orders */
    static const unsigned int digit_order[10]   = {1,9,5,3,7,2,8,4,0,6};
    /* For numbers with ≥ 2 digits, leading digit order
     * 9,5,3,1,7,2,8,4,6   (0 is omitted because it cannot lead).   */
    static const unsigned int leading_order[9]  = {9,5,3,1,7,2,8,4,6};
    static const unsigned int inner_order[10]   = {0,1,9,5,3,7,2,8,4,6};

    /* Single‑digit case: simply return the digit from digit_order. */
    if (n < 10)
        return (seq_t)digit_order[(unsigned)n];

    /* Find the length L of the number and the cumulative count of numbers
     * with fewer digits.  For L ≥ 2 the count of numbers of that length is
     * 9 * 10^(L-1). */
    idx_t cumulative = 10;      /* count of 1‑digit numbers */
    idx_t power      = 10;      /* 10^(L-1) for current L */
    int  L           = 2;       /* length under consideration */

    while (1)
    {
        idx_t cnt = 9 * power;               /* numbers of length L */
        if (n < cumulative + cnt)            /* n lies in this block */
            break;
        cumulative += cnt;
        power *= 10;                          /* advance to next length */
        ++L;
    }

    /* Offset of n inside the block of numbers of length L. */
    idx_t offset = n - cumulative;

    /* Build the result digit by digit. */
    seq_t result = 0;

    /* Leading digit */
    idx_t lead_idx = offset / power;          /* 0 .. 8 */
    unsigned int lead_digit = leading_order[(unsigned)lead_idx];
    result = (seq_t)lead_digit;

    /* Remaining digits */
    idx_t rem = offset % power;               /* residual offset for lower digits */
    for (int pos = 2; pos <= L; ++pos)
    {
        idx_t div = power / 10;
        unsigned int idx = (rem / div) % 10;  /* 0 .. 9 */
        unsigned int d   = inner_order[(unsigned)idx];
        result = result * 10 + (seq_t)d;
        rem    = rem % div;
        power  = div;
    }

    return result;
}
