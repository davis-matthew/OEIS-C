/*
 * A092056  Square table read by downward antidiagonals where T(n,k) =
 *          binomial(n+2^k-1,n).
 * https://oeis.org/A092056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the entry of the table T(n,k)=binomial(n+2^k-1,n) read
 *            by downward antidiagonals. For an index n, find (i,j) such that
 *            n = i + j + (j–i)(j–i+1)/2 and return T(i,j)
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binom_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom_big.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t d = low;

    unsigned __int128 prev128 = (unsigned __int128)d * (d + 1) / 2;
    idx_t prev = (idx_t)prev128;

    idx_t pos = n - prev;         /* position on diagonal d */
    idx_t i   = pos;              /* row index  */
    idx_t j   = d - pos;          /* column index */

    /* Compute T(i,j) = binomial(i + 2^j - 1, i) */
    seq_t power = 1;
    power <<= j;                  /* power = 2^j as seq_t */
    seq_t a_val = (seq_t)i + (power - 1);

    return binom_big(a_val, i);
}
