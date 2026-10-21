/*
 * A096334  Triangle read by rows: T(n,k) = prime(n)#/prime(k)#, 0<=k<=n.
 * https://oeis.org/A096334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = product of consecutive primes in the triangle * T(r,k) =
 *            ∏_{j=k}^{r-1} prime(j+1) , where r is the row * containing the
 *            linear index n and k = n - r(r+1)/2. * (primes are 1‑based:
 *            prime(1)=2, prime(2)=3, …). * Reference: OEIS A096334 – Triangle
 *            read by rows
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 135,
 *            u128 n <= 377, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Index of the first element of row r */
    idx_t before_row = r * (r + 1) / 2;

    /* Column index inside the row */
    idx_t k = n - before_row;

    /* Compute the product of primes from index k to r-1 (0‑based for row,
     * 1‑based for get_prime). */
    seq_t prod = 1;
    for (idx_t i = k; i < r; ++i)
        prod *= (seq_t)get_prime(i + 1);

    return prod;
}
