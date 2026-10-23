/*
 * A145603  a(n) is the number of walks from (0,0) to (0,4) that remain in the
 *          upper half-plane y >= 0 using 2*n +2 unit steps either up (U),
 *          down (D), left (L) or right (R).
 * https://oeis.org/A145603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5/(2*n+3))*binomial(2*n+3,n+4)*binomial(2*n+3,n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 20,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, return 0 */

    idx_t two_n_plus_three = 2 * n + 3;          /* 2n+3 */
    idx_t k = n + 4;                             /* n+4 */
    seq_t b = binomial(two_n_plus_three, k);     /* C(2n+3, n+4) */

    /* Use 128–bit arithmetic to avoid overflow on 64‑bit seq_t */
    unsigned __int128 prod = (unsigned __int128)b * (unsigned __int128)b; /* b^2 */
    prod *= 5;                             /* *5 */
    prod /= two_n_plus_three;              /* / (2n+3) */

    return (seq_t)prod;
}
