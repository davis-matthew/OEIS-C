/*
 * A060982  a(n) = Smallest nontrivial number k > 9 such that |first
 *          (leftmost) decimal digit of k - second digit + third digit -
 *          fourth digit ...| = n.
 * https://oeis.org/A060982
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 45, u64 n <= 90, u128 n <= 174,
 *            big any n
 * Helpers:   get_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A060982.  For n <= 8 it brute‑searches the smallest
 * k > 9 with |alternating digit sum| == n.  For n >= 9 it constructs
 * the minimal number using the pattern described in the OEIS entry:
 *   let r = n % 9, t = (n - r) / 9.
 *   If r == 0:
 *     - if t == 1: return 90.
 *     - else: start with 9 and append (t-1) copies of "09".
 *   If r > 0:
 *     start with r and append t copies of "09".
 */

#include <stddef.h>
#include "../utils/get_digits.h"

static int alt_sum_abs(seq_t x)
{
    uint8_t buf[32];
    size_t len;
    get_digits(x, buf, &len);
    int sum = 0;
    int sign = 1;
    for (size_t i = 0; i < len; ++i)
    {
        sum += sign * buf[i];
        sign = -sign;
    }
    return sum >= 0 ? sum : -sum;
}

seq_t a(idx_t n)
{
    if (n < 9)
    {
        seq_t k = 10;
        while (1)
        {
            if (alt_sum_abs(k) == (int)n)
                return k;
            k++;
        }
    }
    /* n >= 9 */
    idx_t r = n % 9;          /* 0 <= r < 9 */
    idx_t t = (n - r) / 9;    /* number of 09 blocks (or 90 pair) */

    if (r == 0)
    {
        if (t == 1)
            return 90;        /* special case n = 9 */
        seq_t k = 9;
        for (idx_t i = 0; i < t - 1; ++i)
        {
            k = k * 10 + 0;
            k = k * 10 + 9;
        }
        return k;
    }
    else
    {
        seq_t k = r;
        for (idx_t i = 0; i < t; ++i)
        {
            k = k * 10 + 0;
            k = k * 10 + 9;
        }
        return k;
    }
}
