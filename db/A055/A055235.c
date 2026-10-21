/*
 * A055235  Sums of two powers of 3.
 * https://oeis.org/A055235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 3^⌊(1+√(1+8n))/2⌋ + 3^(n -
 *            ⌊(1+√(1+8n))/2⌋(⌊(1+√(1+8n))/2⌋+1)/2)
 * Range:     u8 n <= 17, u16 n <= 62, u32 n <= 228, u64 n <= 859,
 *            u128 n <= 3320, big any n
 * Helpers:   pow3, triangular_inverse
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*     computed by mapping n to the pair (t,m) where t = floor((√(1+8n)-1)/2),
 *     m = n - t*(t+1)/2.  Then a(n) = 3^t + 3^m. */

#include "../utils/pow3.h"          /* provides seq_t pow3(idx_t) */
#include "../utils/triangular_inverse.h"

seq_t a(idx_t n)
{
    idx_t t = triangular_inverse(n);
    /* compute m = n - t*(t+1)/2 using 128–bit to avoid overflow */
    __uint128_t tri = ( (__uint128_t)t * (t + 1) ) / 2;
    idx_t m = (idx_t)(( (__uint128_t)n - tri ));
    return pow3(t) + pow3(m);
}
