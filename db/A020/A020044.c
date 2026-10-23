/*
 * A020044  a(n) = round(Gamma(n+1/3)/Gamma(1/3)).
 * https://oeis.org/A020044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = round(Gamma(n+1/3)/Gamma(1/3))
 *            = round( Π_{k=0}^{n-1} (3k+1) / 3^n )
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use 128‑bit intermediates when seq_t is a native unsigned type
       (sizes 8, 16, 32, or 64 bits).  In that case the product fits
       into 128 bits for all n where the final result fits into seq_t.
       For arbitrary‑precision seq_t types (e.g. boost::multiprecision)
       the computation is performed with seq_t arithmetic directly. */
    if (sizeof(seq_t) <= 8) {
        /* 128‑bit arithmetic path */
        __int128 numer = 1;   /* Π (3k+1)                */
        __int128 denom = 1;   /* 3^n                     */
        for (idx_t k = 0; k < n; ++k) {
            numer *= (__int128)(3 * k + 1);
            denom *= (__int128)3;
        }
        /* round(numer/denom) = floor((numer + denom/2)/denom) */
        __int128 half   = denom / 2;
        __int128 sum    = numer + half;
        __int128 result = sum / denom;
        return (seq_t)result;
    } else {
        /* arbitrary‑precision path */
        seq_t numer = 1;   /* Π (3k+1)                */
        seq_t denom = 1;   /* 3^n                     */
        for (idx_t k = 0; k < n; ++k) {
            numer *= (seq_t)(3 * k + 1);
            denom *= (seq_t)3;
        }
        /* round(numer/denom) = floor((numer + denom/2)/denom) */
        seq_t half   = denom / 2;
        seq_t sum    = numer + half;
        seq_t result = sum / denom;
        return result;
    }
}
