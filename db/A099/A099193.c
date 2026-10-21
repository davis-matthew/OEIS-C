/*
 * A099193  a(n) = n*(4*n^6 + 70*n^4 + 196*n^2 + 45)/315.
 * https://oeis.org/A099193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(4*n^6 + 70*n^4 + 196*n^2 + 45)/315
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 44, u64 n <= 1054,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A099193, offset 0 */
seq_t a(idx_t n)
{
    /* Direct formula using 128‑bit intermediates to avoid overflow */
    if (n == 0)
        return 0;

    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;          /* n^2 */
    unsigned __int128 n4  = n2 * n2;          /* n^4 */
    unsigned __int128 n6  = n4 * n2;          /* n^6 */
    unsigned __int128 p   = 4 * n6 + 70 * n4 + 196 * n2 + 45;
    unsigned __int128 num = nn * p;           /* n * polynomial */
    unsigned __int128 res = num / 315;

    return (seq_t)res;
}
