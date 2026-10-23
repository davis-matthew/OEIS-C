/*
 * A154715  Triangle interpolating between the subsets of an n-set (A000079)
 *          and the trees on n labeled nodes (A000272) (read by rows).
 * https://oeis.org/A154715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(row, col)*(col+2)^row where row = largest
 *            integer such that row*(row+1)/2 <= n and col = n - row*(row+1)/2
 * Range:     u8 n <= 10, u16 n <= 23, u32 n <= 49, u64 n <= 128,
 *            u128 n <= 365, big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find row r */
    idx_t r = 0;
    while (r * (r + 1) / 2 <= n)
        r++;
    r--;                          /* r now satisfies r*(r+1)/2 <= n */

    idx_t start = r * (r + 1) / 2;
    idx_t col   = n - start;       /* 0 <= col <= r */

    seq_t bin   = binomial(r, col);            /* C(r, col) */
    seq_t powr  = big_pow((seq_t)(col + 2), (seq_t)r); /* (col+2)^r */

    return bin * powr;               /* a(n) */
}
