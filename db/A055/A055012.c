/*
 * A055012  Sum of cubes of the digits of n written in base 10.
 * https://oeis.org/A055012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy, look
 * Formula:   a(n) = sum of cubes of the decimal digits of n
 * Range:     u8 n <= 6, u16 n <= 9535, u32 n <= 9535, u64 n <= 9535,
 *            u128 n <= 9535, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const seq_t cube[10] = {
        0ULL,  // 0^3
        1ULL,  // 1^3
        8ULL,  // 2^3
        27ULL, // 3^3
        64ULL, // 4^3
        125ULL,// 5^3
        216ULL,// 6^3
        343ULL,// 7^3
        512ULL,// 8^3
        729ULL // 9^3
    };

    seq_t sum = 0;
    idx_t temp = n;

    if (temp == 0)
        return 0;

    while (temp)
    {
        /* Extract least significant decimal digit. */
        int d = temp % 10;
        sum += cube[d];
        temp /= 10;
    }
    return sum;
}
