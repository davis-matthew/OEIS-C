/*
 * A095695  T(n,4) diagonal of triangle in A095693.
 * https://oeis.org/A095695
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n-1)*(n-2)*(n-3)*(n^4+2*n^3-13*n^2-54*n+136)/384
 *            (valid for n >= 4)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 4) return 0;          /* undefined for n < 4 */

    /* Use 128‑bit intermediate to avoid overflow for 64‑bit builds */
    unsigned __int128 nn   = (unsigned __int128)n;
    unsigned __int128 a4   = nn * (nn - 1) * (nn - 2) * (nn - 3);

    /* polynomial part: n^4 + 2*n^3 - 13*n^2 - 54*n + 136 */
    unsigned __int128 n2   = nn * nn;
    unsigned __int128 n3   = n2 * nn;
    unsigned __int128 n4   = n3 * nn;
    unsigned __int128 poly = n4
                           + ((unsigned __int128)2) * n3
                           - ((unsigned __int128)13) * n2
                           - ((unsigned __int128)54) * nn
                           + ((unsigned __int128)136);

    unsigned __int128 res = a4 * poly;
    res /= 384;  /* division is exact */

    return (seq_t)res;   /* truncation is safe for all n where the result fits in seq_t */
}
