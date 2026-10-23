/*
 * A038296  Triangle whose (i,j)-th entry is binomial(i,j)*9^(i-j)*6^j.
 * https://oeis.org/A038296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(r,c) * 9^(r-c) * 6^c, where r =
 *            floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2. The sequence is the
 *            triangle T(i,j) = binomial(i,j)*9^(i-j)*6^j flattened row‑major
 * Range:     u8 n <= 5, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   binomial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include <stdint.h>

static idx_t _row_from_index(idx_t n)
{
    idx_t low = 0;
    idx_t high = 1;
    /* find an upper bound for the row index */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* binary search for the largest i with i*(i+1)/2 <= n */
    while (low < high) {
        idx_t mid = low + ((high - low + 1) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Determine the row (i) and column (j) of the term. */
    idx_t i = _row_from_index(n);
    unsigned __int128 start128 = (unsigned __int128)i * (i + 1) / 2;
    idx_t start = (idx_t)start128;
    idx_t j = n - start;

    /* Compute the term: binomial(i,j) * 9^(i-j) * 6^j */
    seq_t bin = binomial(i, j);
    seq_t pow9 = pow_uint(9, i - j);
    seq_t pow6 = pow_uint(6, j);

    seq_t result = bin;
    result *= pow9;
    result *= pow6;

    return result;
}
