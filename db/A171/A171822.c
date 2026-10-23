/*
 * A171822  Triangle T(n,k) = binomial(2*n-k, k)*binomial(n+k, 2*k), read by
 *          rows.
 * https://oeis.org/A171822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle T read by rows, where
 *            T(m,k) = binomial(2*m-k, k) * binomial(m+k, 2*k)
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 110, u64 n <= 361,
 *            u128 n <= 1248, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Offset within the row.  k = n - r*(r+1)/2. */
    idx_t k = n - (r * (r + 1)) / 2;

    /* Compute the two binomial coefficients. */
    seq_t a1 = binomial(2 * r - k, k);
    seq_t a2 = binomial(r + k, 2 * k);

    /* Return their product.  For built‑in types seq_mul truncates
       as required, while for arbitrary precision it gives the exact
       value. */
    return seq_mul(a1, a2);
}
