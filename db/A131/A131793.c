/*
 * A131793  3 odds, 3 evens.
 * https://oeis.org/A131793
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(3*floor((n-1)/6)+((n-1) mod 3)+1)-(floor((n+2)/3) mod
 *            2)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute floor((n-1)/6) and (n-1) mod 3 */
    idx_t base_div6 = (n - 1) / 6u;
    idx_t base_mod3 = (n - 1) % 3u;

    /* Compute floor((n+2)/3) without overflowing n+2 */
    idx_t floor_plus2_div3 = (n / 3u) + ((n % 3u + 2u) / 3u);

    /* Assemble the formula */
    idx_t result = 2u * (3u * base_div6 + base_mod3 + 1u) - (floor_plus2_div3 % 2u);

    return (seq_t)result;
}
