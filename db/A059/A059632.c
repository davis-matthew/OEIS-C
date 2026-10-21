/*
 * A059632  Carryless product 11 X n base 10.
 * https://oeis.org/A059632
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = carryless sum of n and n*10. Equivalent to a(n) = Σ (d_i
 *            + d_{i-1}) * 10^i with digits d_i of n (d_{-1}=0)
 * Range:     u8 n <= 23, u16 n <= 5999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t result = 0;      /* accumulated value */
    seq_t pos    = 1;      /* current 10^i */

    /* previous digit of n (d_{i-1}) */
    int8_t prev = 0;

    /* Process each decimal digit of n from least significant upward. */
    while (n > 0 || prev > 0) {
        /* current digit d_i of n */
        int8_t d = (int8_t)(n % 10);

        /* carryless addition digit: (d_i + d_{i-1}) mod 10 */
        int8_t ad = (d + prev) % 10;

        result += (seq_t)ad * pos;
        pos    *= 10;

        n   /= 10;
        prev = d;          /* for the next higher position */
    }

    return result;
}
