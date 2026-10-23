/*
 * A036561  Nicomachus triangle read by rows, T(n, k) = 2^(n - k)*3^k, for 0
 *          <= k <= n.
 * https://oeis.org/A036561
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl, nice
 * Formula:   a(n) = 2^(r-k) * 3^k, where r = ⌊(√(8n+1)-3)/2⌋ and k = n -
 *            r(r+1)/2
 * Range:     u8 n <= 24, u16 n <= 74, u32 n <= 249, u64 n <= 900,
 *            u128 n <= 3401, big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* approximate the row index r */
    long double d = sqrt((long double)n * 8.0L + 1.0L);
    uint64_t r_ull = (uint64_t)((d - 1.0L) / 2.0L);

    /* adjust r if needed so that n < C(r) where C(r) = (r+1)(r+2)/2 */
    unsigned __int128 curr = ((unsigned __int128)(r_ull + 1) * (r_ull + 2)) / 2;
    if (n >= (idx_t)curr) {
        r_ull++;
    }

    /* compute k = position within the row */
    unsigned __int128 prev = ((unsigned __int128)r_ull * (r_ull + 1)) / 2;
    idx_t k = n - (idx_t)prev;

    /* calculate the term: 2^(r-k) * 3^k */
    seq_t val = pow2((idx_t)(r_ull - k));
    val = val * pow3(k);
    return val;
}
