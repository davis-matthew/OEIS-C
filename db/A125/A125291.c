/*
 * A125291  Number of partitions of n into positive digit values of its
 *          ternary representation.
 * https://oeis.org/A125291
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 if the base‑3 digit set of n contains only 1 or only 2
 *            a(n) = floor(n/2) + 1 otherwise (both digits 1 and 2 appear)
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    bool seen1 = false;
    bool seen2 = false;
    idx_t temp = n;

    while (temp > 0)
    {
        unsigned digit = temp % 3U;
        if (digit == 1U) seen1 = true;
        else if (digit == 2U) seen2 = true;
        if (seen1 && seen2) break;   /* both digits found, can stop early */
        temp /= 3U;
    }

    if (!(seen1 && seen2))
        return (seq_t)1U;

    /* floor(n/2) + 1 */
    return (seq_t)(n / 2U + 1U);
}
