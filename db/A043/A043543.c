/*
 * A043543  Number of distinct base-16 digits of n.
 * https://oeis.org/A043543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of distinct hexadecimal digits in n
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset: a(1) is the first term. */
    if (n == 0) return (seq_t)0;

    uint64_t mask = 0;
    uint64_t value = n;
    do
    {
        uint64_t digit = value & 0xF;            /* value % 16 */
        mask |= (uint64_t)1 << digit;            /* mark digit */
        value >>= 4;                             /* value /= 16 */
    } while (value);

    /* Count bits set in mask (at most 16 bits). */
    unsigned int count = 0;
    uint16_t m = (uint16_t)mask;
    while (m)
    {
        count++;
        m &= m - 1;               /* clear lowest set bit */
    }
    return (seq_t)count;
}
