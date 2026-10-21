/*
 * A063264  Tenth column (k=9) of sextinomial array A063260.
 * https://oeis.org/A063264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(n+4)*((n^5 + 44*n^4 + 791*n^3 +
 *            7384*n^2 + 37140*n + 30240))/9!
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 42, u64 n <= 567, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* For uint64_t, used only if seq_t is that type. */

seq_t a(idx_t n)
{
    /* Compute in 128‑bit to avoid intermediate overflow. */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n5 = n4 * nn;          /* n^5 */

    unsigned __int128 poly = n5;
    poly += (unsigned __int128)44 * n4;
    poly += (unsigned __int128)791 * n3;
    poly += (unsigned __int128)7384 * n2;
    poly += (unsigned __int128)37140 * nn;
    poly += (unsigned __int128)30240;

    unsigned __int128 prod = (nn + 1) * (nn + 2) * (nn + 3) * (nn + 4);

    unsigned __int128 numerator = prod * poly;

    /* 9! = 362880.  The division is exact. */
    const unsigned __int128 denom = 362880;
    unsigned __int128 result = numerator / denom;

    return (seq_t)result;
}
