/*
 * A095143  Triangle, read by rows, formed by reading Pascal's triangle
 *          (A007318) mod 9.
 * https://oeis.org/A095143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(row_of_index(n), n - row_prefix(n)) mod 9 where
 *            the triangle is read row by row from Pascal's triangle
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   binomial_mod9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial_mod9.h"

/* Find the row index i such that  i*(i+1)/2 <= n < (i+1)*(i+2)/2. */
static inline idx_t
row_of_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    /* First grow an upper bound. */
    while (true) {
        unsigned __int128 t = (unsigned __int128)high * (high + 1) / 2;
        if (t > n) break;
        high <<= 1;
    }
    low = high >> 1;
    /* Binary search between low and high. */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    return low - 1;
}

/* OEIS sequence A095143. */
seq_t
a(idx_t n)
{
    idx_t row = row_of_index(n);
    /* prefix = sum_{i=0}^{row-1} (i+1) = row*(row+1)/2  */
    idx_t prefix = (idx_t)((unsigned __int128)row * (row + 1) / 2);
    idx_t col = n - prefix;                /* column index within the row */
    return binomial_mod9(row, col);
}
