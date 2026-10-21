/*
 * A089815  a(n) = floor((n+2)^(n+2)/((n+2)^2-1)).
 * https://oeis.org/A089815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(((n+2)^(n+2))/((n+2)^2-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include "../utils/big_pow.h"
seq_t a(idx_t n)
{
    seq_t m = (seq_t)(n + 2);
    seq_t denom = m * m - 1;
    seq_t num = big_pow(m, m);
    return num / denom;
}
#else
seq_t a(idx_t n)
{
    unsigned __int128 m = (unsigned __int128)(n + 2);
    unsigned __int128 denom = m * m - 1;
    unsigned __int128 num = 1;
    for (idx_t i = 0; i < n + 2; ++i)
        num *= m;
    unsigned __int128 res = num / denom;
    return (seq_t)res;
}
#endif
