/*
 * A106034  a(n) is the least number such that n*prime(n)+a(n) is a perfect
 *          cube.
 * https://oeis.org/A106034
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k ≥ 0 such that n*prime(n)+k is a perfect cube
 * Range:     u8 n <= 6, u16 n <= 26, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* base = n * prime(n) */
    seq_t base = (seq_t)n * get_prime(n);

    /* Binary search for the smallest m with m^3 ≥ base */
    seq_t low = 0;
    seq_t high = base;   /* safe upper bound for the cube root */
    while (low < high) {
        seq_t mid = (low + high) / 2;
        seq_t mid2 = mid * mid;
        seq_t mid3 = mid2 * mid;
        if (mid3 < base) {
            low = mid + 1;
        } else {
            high = mid;
        }
    }

    /* low is now the cube root floor, and low^3 ≥ base */
    seq_t cube = low * low * low;
    return cube - base;
}
