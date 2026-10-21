/*
 * A090394  Ninth diagonal (m=8) of triangle A084938; a(n) = A084938(n+8,n) =
 *          (n^8 + 84*n^7 + 3346*n^6 + 84840*n^5 + 1550689*n^4 + 21632436*n^3
 *          + 224782284*n^2 + 1377648720*n)/40320.
 * https://oeis.org/A090394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^8 + 84*n^7 + 3346*n^6 + 84840*n^5 + 1550689*n^4 +
 *            21632436*n^3 + 224782284*n^2 + 1377648720*n)/40320 (n ≥ 0)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 49, u64 n <= 953, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* coefficients for powers of n from n^0 to n^8 */
    static const seq_t coeffs[9] = {
        0ULL,              /* n^0 */
        1377648720ULL,     /* n^1 */
        224782284ULL,      /* n^2 */
        21632436ULL,       /* n^3 */
        1550689ULL,        /* n^4 */
        84840ULL,          /* n^5 */
        3346ULL,           /* n^6 */
        84ULL,             /* n^7 */
        1ULL               /* n^8 */
    };

    /* use 128‑bit arithmetic for the intermediate result */
    unsigned __int128 res = 0;
    unsigned __int128 pow = 1; /* n^0 */
    unsigned __int128 n128 = n;

    for (int k = 0; k <= 8; ++k) {
        res += (unsigned __int128)coeffs[k] * pow;
        pow *= n128;  /* next power of n */
    }

    /* division by 40320 is exact (factorial 8!) */
    return (seq_t)(res / 40320);
}
