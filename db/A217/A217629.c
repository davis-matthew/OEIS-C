/*
 * A217629  Triangle, read by rows, where T(n,k) = k!*C(n, k)*3^(n-k) for
 *          n>=0, k=0..n.
 * https://oeis.org/A217629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8n+1)-1)/2), k = n -
 *            r(r+1)/2 and T(r,k) = k!*C(r,k)*3^(r-k) = 3^(r-k) * r!/(r-k)!
 * Range:     u8 n <= 15, u16 n <= 38, u32 n <= 97, u64 n <= 244,
 *            u128 n <= 623, big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Find row r such that r*(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ( ((unsigned __int128)(r + 1) * (r + 2) / 2) <= n ) {
        r++;
    }
    /* Compute column k */
    idx_t triangular = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - triangular;

    /* Compute r!/(r-k)! = product_{i=0}^{k-1} (r-i) */
    seq_t result = 1;
    for (idx_t i = 0; i < k; ++i) {
        result *= (unsigned long long)(r - i);
    }

    /* Multiply by 3^(r-k) */
    result *= pow3(r - k);

    return result;
}
