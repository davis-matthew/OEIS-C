/*
 * A108747  Triangle read by rows: T(n,k) is the number of Grand Dyck paths of
 *          semilength n and having k returns to the x-axis.
 * https://oeis.org/A108747
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(n,k) defined by *
 *            T(n,k) = k * 2^k * binomial(2n-k,n) / (2n-k) (1 ≤ k ≤ n). * The
 *            triangle is read by rows, starting with a(1)=T(1,1)
 * Range:     u8 n <= 11, u16 n <= 37, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"     /* binomial(idx_t N, idx_t K)        */
#include "../utils/pow2.h"         /* pow2(idx_t exp)                   */

seq_t a(idx_t n)
{
    /* a(1) is the first defined term; values for n < 1 are out of range. */
    if (n < 1)
        return 0;

    /* Find the row r such that r*(r+1)/2 ≥ n and (r-1)*r/2 < n. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Number of elements before row r. */
    idx_t prev = (r - 1) * r / 2;
    /* Column index k (1‑based) within the found row. */
    idx_t k = n - prev;

    /* Compute T(r,k) = k * 2^k * binomial(2r-k, r) / (2r-k). */
    idx_t denom = 2 * r - k;
    seq_t bin = binomial(2 * r - k, r);
    seq_t two_pow_k = pow2(k);

    seq_t val = bin * two_pow_k;  /* bin * 2^k */
    val *= k;                     /* * k */
    val /= (seq_t)denom;          /* / (2r-k) */

    return val;
}
