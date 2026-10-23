/*
 * A156367  Triangle T(n, k) = binomial(n+k, 2*k)*k!, read by rows.
 * https://oeis.org/A156367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = flattening of the triangle T(r,c) = binomial(r+c,2c)*c!
 *            read by rows
 * Range:     u8 n <= 23, u16 n <= 42, u32 n <= 100, u64 n <= 247,
 *            u128 n <= 625, big any n
 * Helpers:   _my_fact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_my_fact.h"

/* local helpers – use seq_t arithmetic */
static seq_t _my_binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    idx_t r = 0;
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t offset = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - offset;

    seq_t res = _my_binom(r + c, 2 * c);
    res *= _my_fact(c);
    return res;
}
