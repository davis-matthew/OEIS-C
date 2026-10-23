/*
 * A038332  Triangle whose (i,j)-th entry is binomial(i,j)*12^(i-j)*6^j.
 * https://oeis.org/A038332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j)*12^(i-j)*6^j where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2 (row‑major
 *            enumeration of the triangle)
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 139, u128 n <= 533,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_int.h"

static idx_t find_row(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t val = (__uint128_t)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    idx_t i = find_row(n);
    __uint128_t prev128 = (__uint128_t)i * (i + 1) / 2;
    idx_t prev = (idx_t)prev128;
    idx_t j = n - prev;

    seq_t res = binomial(i, j);
    res *= pow_int(12, i - j);
    res *= pow_int(6, j);
    return res;
}
