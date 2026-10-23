/*
 * A040523  Continued fraction for sqrt(547).
 * https://oeis.org/A040523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A040523: continued fraction partial quotients for sqrt(547)
 *
 * The sequence is periodic after the initial term a(0)=23.  The period
 * consists of 26 terms:
 *   2, 1, 1, 2, 1, 2, 1, 7, 15, 2, 6, 5, 23, 5, 6, 2,
 *   15, 7, 1, 2, 1, 2, 1, 1, 2, 46
 *
 * For any index n ≥ 0, a(n) is returned as a seq_t.  The function
 * uses a static array to store the period and returns the appropriate
 * element modulo the period length.
 */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)23;

    /* Period of the continued fraction for sqrt(547) */
    static const unsigned short period[] =
    {
        2, 1, 1, 2, 1, 2, 1, 7, 15, 2, 6, 5, 23, 5, 6, 2,
        15, 7, 1, 2, 1, 2, 1, 1, 2, 46
    };
    const size_t L = sizeof(period) / sizeof(period[0]);

    /* index into the period array */
    size_t idx = (size_t)((n - 1) % L);
    return (seq_t)period[idx];
}
