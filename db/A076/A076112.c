/*
 * A076112  Triangle (read by rows) in which the n-th row contains first n
 *          terms of n geometric progression with first term 1 and common
 *          ratio (n-1).
 * https://oeis.org/A076112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = (t+1) ^ ( n - t*(t+1)/2 - 1 ), where t = floor( (-1 +
 *            sqrt(8*n-7)) / 2 )
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 152,
 *            u128 n <= 405, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t b = base;
    while (exp)
    {
        if (exp & 1)
            result *= b;
        exp >>= 1;
        if (exp)
            b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* under the offset, value does not matter */

    /* Find the row r such that r*(r+1)/2 >= n   (1‑based rows) */
    idx_t low  = 1, high = 1;
    /* Expand high until it satisfies the condition */
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;
    while (low < high)
    {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t row = low;                           /* row number (1‑based) */
    idx_t prev = (row - 1) * row / 2;           /* number of terms before this row */

    idx_t exp   = n - prev - 1;                /* exponent within the row (0‑based) */
    seq_t base  = (seq_t)(row);                /* base of the geometric progression */

    return pow_seq(base, exp);
}
