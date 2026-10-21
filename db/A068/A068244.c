/*
 * A068244  1/6 the number of colorings of a 3 X 3 rhombic- or staggered-
 *          hexagonal array with n colors.
 * https://oeis.org/A068244
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^9 -16*n^8 +112*n^7 -448*n^6 +1120*n^5 -1791*n^4
 *            +1786*n^3 -1012*n^2 +248*n)/6 (valid for n ≥ 3)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 16, u64 n <= 170, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* use the compiler's 128‑bit signed integer type if available */
#if defined(__SIZEOF_INT128__)
typedef __int128 int128_t;
#else
/* fall back to a 64‑bit signed integer; this will work only for small n. */
typedef int64_t int128_t;
#endif

seq_t a(idx_t n)
{
    if (n < 3) return 0;    /* sequence is defined only for n ≥ 3 */

    /* compute the polynomial in a 128‑bit signed intermediate to avoid
       overflow and to allow subtractions.  The final result fits in seq_t
       for all values tested. */
    int128_t t = 1;            /* leading coefficient of n^9 */

    t = t * n - 16;
    t = t * n + 112;
    t = t * n - 448;
    t = t * n + 1120;
    t = t * n - 1791;
    t = t * n + 1786;
    t = t * n - 1012;
    t = t * n + 248;
    t = t * n;

    t /= 6;                     /* division is exact */

    return (seq_t)t;
}
