/*
 * A046072  Decompose multiplicative group of integers modulo n as a product
 *          of cyclic groups C_{k_1} x C_{k_2} x ... x C_{k_m}, where k_i
 *          divides k_j for i < j; then a(n) = m.
 * https://oeis.org/A046072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A001221(n) - 1 if n > 2 is divisible by 2 and not by 4,
 *            a(n) = A001221(n) + 1 if n is divisible by 8, a(n) = A001221(n)
 *            in other cases. Here A001221(n) is the number of distinct prime
 *            factors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the small special cases first */
    if (n == 1)
        return (seq_t)1;

    /* Count distinct prime factors of n */
    uint64_t temp = (uint64_t)n;
    uint64_t distinct = 0;

    if (temp % 2 == 0) {
        distinct++;
        while (temp % 2 == 0)
            temp /= 2;
    }

    for (uint64_t p = 3; p * p <= temp; p += 2) {
        if (temp % p == 0) {
            distinct++;
            while (temp % p == 0)
                temp /= p;
        }
    }

    if (temp > 1)
        distinct++;

    seq_t result = (seq_t)distinct;

    /* Apply the special adjustments described in the formula */
    if (n > 2 && (n & 1) == 0 && (n & 3) != 0) {
        /* n divisible by 2 but not by 4 */
        result -= 1;
    } else if ((n & 7) == 0) {
        /* n divisible by 8 */
        result += 1;
    }

    return result;
}
