/*
 * A038319  Triangle whose (i,j)-th entry is binomial(i,j)*11^(i-j)*5^j.
 * https://oeis.org/A038319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = binomial(i,j) * 11^(i-j) * 5^j, where i =
 *            floor((sqrt(8*n+1)-1)/2) and j = n - i*(i+1)/2
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 45, u64 n <= 155, u128 n <= 567,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

static idx_t find_row(idx_t n)
{
    /* Binary search for i such that (i+1)(i+2)/2 > n >= i(i+1)/2. */
    idx_t low = 0, high = 1;
    /* Expand high until the triangular bound exceeds n. */
    while (1) {
        unsigned __int128 tri = (unsigned __int128)high * (high + 1) / 2;
        if (tri > n || high == UINT64_MAX) break;
        high = high << 1;
        if (high == UINT64_MAX) { high = UINT64_MAX; break; }
    }
    /* Narrow the interval. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) low = mid + 1;
        else          high = mid;
    }
    return low - 1; /*
                    * low is the first index with triangular > n,
                    * so the desired i is low-1.
                    */
}

static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    idx_t i = find_row(n);
    idx_t row_start = i * (i + 1) / 2;
    idx_t j = n - row_start;

    seq_t bin = binomial(i, j);
    seq_t pow11 = pow_seq((seq_t)11, i - j);
    seq_t pow5  = pow_seq((seq_t)5, j);

    return bin * pow11 * pow5;
}
