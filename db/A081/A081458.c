/*
 * A081458  Table T(m,n) = (3^m + 5^n)/2, for m, n = 0, 2, 4, 6, ... read by
 *          antidiagonals downwards.
 * https://oeis.org/A081458
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = ((9)^p + (25)^q)/2 where s = ⌊(√(8n + 1) - 1)/2⌋, offset
 *            = n - s*(s+1)/2, p = offset, q = s - offset. This follows the
 *            OEIS A081458 definition: Table T(m,n) = (3^m + 5^n)/2 for m,n =
 *            0,2,4,…, read by antidiagonals downwards
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 27, u64 n <= 104, u128 n <= 405,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Compute s = floor((sqrt(8n + 1) - 1) / 2) */
    long double D = sqrtl( (long double)8 * (long double)n + 1.0L );
    idx_t s = (idx_t)((D - 1.0L) / 2.0L);

    /* Ensure s is correct (correct for any fp round‑off) */
    unsigned __int128 before128 = (unsigned __int128)s * (s + 1) / 2;
    idx_t before = (idx_t)before128;
    while (before > n) {
        --s;
        before128 = (unsigned __int128)s * (s + 1) / 2;
        before = (idx_t)before128;
    }
    while ( (unsigned __int128)(s + 1) * (s + 2) / 2 <= n ) {
        ++s;
        before128 = (unsigned __int128)s * (s + 1) / 2;
        before = (idx_t)before128;
    }

    idx_t offset = n - before;   /* 0 <= offset <= s */
    idx_t p = offset;
    idx_t q = s - offset;

    seq_t pow9  = big_pow((seq_t)9,  (seq_t)p);
    seq_t pow25 = big_pow((seq_t)25, (seq_t)q);
    return (pow9 + pow25) / 2;
}
