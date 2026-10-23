/*
 * A031035  Write n in base 8 and juxtapose.
 * https://oeis.org/A031035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy, tabf
 * Formula:   a(n) = the n-th digit of the base-8 Champernowne constant *
 *            obtained by concatenating the octal representations of 1, 2, 3,
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find the integer k whose octal digits contain the n-th digit. */
    idx_t cumulative = 0;        /* number of digits processed so far   */
    idx_t k = 1;                 /* current integer to inspect          */

    while (1)
    {
        /* length of k in base 8 */
        idx_t temp = k;
        idx_t len = 1;
        while (temp >= 8)
        {
            temp /= 8;
            ++len;
        }

        if (cumulative + len >= n)
            break;

        cumulative += len;
        ++k;
    }

    /* Determine the offset of the required digit within k's octal string. */
    idx_t offset = n - cumulative - 1; /* 0-indexed within the representation */

    /* Find length of k in base 8 again (caching length to avoid recompute). */
    idx_t temp = k;
    idx_t len = 1;
    while (temp >= 8)
    {
        temp /= 8;
        ++len;
    }

    /* Compute the digit by repeated division. */
    idx_t steps = (len - 1) - offset;   /* number of times we divide by 8 from the most significant digit */
    idx_t value = k;
    while (steps > 0)
    {
        value /= 8;
        --steps;
    }
    return (seq_t)(value % 8);
}
