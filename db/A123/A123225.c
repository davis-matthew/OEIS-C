/*
 * A123225  Triangle read by rows: T(n,d) =
 *          (n!/d!)*(n+1)*binomial(2n-d+1,n+1)/(n-d+1) (0 <= d <= n).
 * https://oeis.org/A123225
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,d) with r = floor((sqrt(1+8n)-1)/2), d = n –
 *            r*(r+1)/2, T(r,d) = (r!/d!) * (r+1) * C(2r-d+1, r+1) / (r-d+1)
 *            (0 ≤ d ≤ r) The triangle is read by rows: 1; 3,2; 20,12,3; …
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 35, u64 n <= 119, u128 n <= 324,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find row r and column d from the flat index n.  */
    idx_t r = 0;
    while (( (__uint128_t)(r + 1) * (r + 2) / 2 ) <= n)
        r++;

    idx_t d = n - (r * (r + 1)) / 2;

    /* Compute factorial ratio r!/d! as ∏_{i=d+1}^{r} i  */
    seq_t ratio = 1;
    for (idx_t i = d + 1; i <= r; ++i)
        ratio *= (seq_t)i;

    /* Binomial part C(2r-d+1, r+1)  */
    seq_t bin = binomial(2 * r - d + 1, r + 1);

    /* Assemble the final value: ratio * (r+1) * bin / (r-d+1) */
    seq_t result = ratio;
    result *= (seq_t)(r + 1);
    result *= bin;
    result /= (seq_t)(r - d + 1);

    return result;
}
