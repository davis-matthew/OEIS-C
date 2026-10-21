/*
 * A204116  Symmetric matrix based on f(i,j) = gcd(2^i-1, 2^j-1), by
 *          antidiagonals.
 * https://oeis.org/A204116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = gcd(2^i-1, 2^j-1) where (i,j) are the coordinates
 *            obtained by traversing the infinite symmetric matrix M with
 *            M(i,j)=gcd(2^i-1, 2^j-1) along antidiagonals
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that the first d terms cover n. */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n)
        ++d;

    /* Number of terms before diagonal d. */
    idx_t prev = ((d - 1) * d) / 2;

    /* Offset within diagonal d (0‑based). */
    idx_t offset = n - prev - 1;

    /* Coordinates (i,j) on diagonal d. */
    idx_t i = offset + 1;
    idx_t j = d - offset;

    /* Compute gcd of indices and then 2^gcd – 1. */
    seq_t g = gcd((seq_t)i, (seq_t)j);
    seq_t val = pow2((idx_t)g);
    return val - 1;
}
