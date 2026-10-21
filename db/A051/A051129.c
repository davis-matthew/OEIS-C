/*
 * A051129  Table T(n,k) = k^n read by upwards antidiagonals (n >= 1, k >= 1).
 * https://oeis.org/A051129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, nice
 * Formula:   a(n) = (n - b(b-1)/2)^(b(b+1)/2 - n + 1), * where b = floor(1/2
 *            + sqrt(2*n)) = min{b | b(b+1)/2 >= n}
 * Range:     u8 n <= 24, u16 n <= 58, u32 n <= 157, u64 n <= 412,
 *            u128 n <= 1092, big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;           /* offset 1 – a(1) is the first term */

    /* Find the unique b such that b*(b+1)/2 >= n. */
    idx_t low = 1, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n)
        high <<= 1;          /* double until the inequality holds */

    while (low < high) {    /* binary search */
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t b = low;

    /* Compute the triangle numbers in a wider type to avoid overflow. */
    unsigned __int128 tri_prev = (unsigned __int128)b * (b - 1) / 2;
    unsigned __int128 tri      = (unsigned __int128)b * (b + 1) / 2;

    /* Base and exponent fit in seq_t because b is small (≈√(2n)). */
    seq_t base = (seq_t)(n - (idx_t)tri_prev);
    seq_t exp  = (seq_t)(tri - n + 1);

    return big_pow(base, exp);
}
