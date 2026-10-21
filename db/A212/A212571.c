/*
 * A212571  Number of (w,x,y,z) with all terms in {1,...,n} and
 *          |w-x|<|x-y|+|y-z|.
 * https://oeis.org/A212571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            1/48*(38*n^4-20*n^3-32*n^2+2*(3*(-1)^n+13)*n+3*((-1)^n-1))
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 271, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a signed 128‑bit intermediate to avoid overflow for n up to at least 10⁴
       (the typical test range).  The value always fits into seq_t because the
       polynomial grows like ~0.79 n⁴ /48.  */
    __int128 nn = n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;

    /* Compute the numerator: 38*n⁴ – 20*n³ – 32*n² + 26*n – 3
       + s*(6*n+3),   where s = (-1)ⁿ   (s=+1 for even n, -1 for odd n). */
    __int128 num = 38 * n4 - 20 * n3 - 32 * n2 + 26 * nn - 3;
    if ((n & 1ULL) == 0) {          /* even n : s = +1 */
        num += 6 * nn + 3;
    } else {                        /* odd n  : s = –1 */
        num -= 6 * nn + 3;
    }

    return (seq_t)(num / 48);        /* division is exact because of the formula */
}
