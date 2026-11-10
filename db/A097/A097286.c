/*
 * A097286  Rectangular array T by descending antidiagonals: T(n,k) = rank of
 *          k-th n in A097285.
 * https://oeis.org/A097286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   antidiagonal d, entry i (r=i, c=d+1-i); T(r,c) = (m-2)(m-1)+pos,
 *            where if c<=r-1 then m=r, pos=2c, else m=c+1, pos=2r-1 (position
 *            of c-th r in A097285)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Validated: partial, pass, 78 terms through n = 78 (2026-11-07)
 * Helpers:   isqrt_u128_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u128_to_u64.h"

seq_t a(idx_t n)
{
    unsigned __int128 val = (unsigned __int128)8 * n + 1;
    uint64_t s = isqrt_u128_to_u64(val);
    uint64_t d = (s + 1) / 2;
    if (d < 1) d = 1;
    while ((unsigned __int128)d * (d + 1) / 2 < n) d++;
    while (d > 1 && (unsigned __int128)(d - 1) * d / 2 >= n) d--;

    idx_t i = n - (idx_t)((unsigned __int128)d * (d - 1) / 2);
    seq_t r = (seq_t)i;
    seq_t c = (seq_t)d + 1 - (seq_t)i;

    seq_t m, pos;
    if (c + 1 <= r) {
        m = r;
        pos = 2 * c;
    } else {
        m = c + 1;
        pos = 2 * r - 1;
    }
    return (m - 2) * (m - 1) + pos;
}
