/*
 * A082022  In the following square array A(i,j) = Least Common Multiple of i
 *          and j. Sequence contains the product of the terms of the n-th
 *          antidiagonal.
 * https://oeis.org/A082022
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} lcm(k, n+1–k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 21,
 *            big any n
 * Helpers:   egcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                /* for uint64_t */
#include "../utils/egcd_uint64.h"

/* OEIS A082022: product of the LCMs along the n‑th anti‑diagonal.  */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;           /* empty product */

    seq_t prod = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t i = k;                     /* first element of the pair */
        idx_t j = n + 1 - k;              /* second element of the pair */

        uint64_t g = egcd_uint64(i, j);   /* gcd(i, j) */

        /* Compute lcm(i, j) = i / gcd(i, j) * j  as a 64‑bit integer. */
        uint64_t l = (i / g) * j;

        prod *= (seq_t)l;                /* accumulate in arbitrary precision */
    }

    return prod;
}
