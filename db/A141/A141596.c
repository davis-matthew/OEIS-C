/*
 * A141596  Triangle T(n,k) = 4*binomial(n,k)^2 - 3, read by rows, 0<=k<=n.
 * https://oeis.org/A141596
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 4*binomial(r,k)^2 − 3, where r = floor((sqrt(8n+1)−1)/2)
 *            and k = n − r(r+1)/2
 * Range:     u8 n <= 16, u16 n <= 58, u32 n <= 178, u64 n <= 610,
 *            u128 n <= 2307, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r   = 0;
    idx_t cum = 0;
    while (((unsigned __int128)cum + (unsigned __int128)(r + 1))
           <= (unsigned __int128)n) {
        cum += (r + 1);
        r++;
    }

    idx_t k = n - cum;                /* position within the r‑th row   */
    seq_t bin = binomial(r, k);       /* binomial coefficient C(r,k)   */

    seq_t val = seq_mul(bin, bin);    /* C(r,k)^2                       */
    val = seq_mul(val, (seq_t)4);     /* 4*C(r,k)^2                     */
    val = val - (seq_t)3;             /* 4*C(r,k)^2 − 3                 */

    return val;
}
