/*
 * A182494  Decimal expansion of 9091/9901.
 * https://oeis.org/A182494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, easy, nonn
 * Formula:   a(n) = the nth digit of the decimal expansion of 9091/9901. The
 *            digits repeat with period 12: [9,1,8,1,9,0,0,8,1,8,0,9]
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Periodic pattern of 12 digits */
    static const seq_t pattern[12] = {
        9ULL, 1ULL, 8ULL, 1ULL, 9ULL, 0ULL,
        0ULL, 8ULL, 1ULL, 8ULL, 0ULL, 9ULL
    };

    /* Compute index modulo period length */
    idx_t r = n % 12;
    return pattern[(size_t)r];
}
