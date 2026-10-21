/*
 * A088617  Triangle read by rows: T(n,k) = C(n+k,n)*C(n,k)/(k+1), for n >= 0,
 *          k = 0..n.
 * https://oeis.org/A088617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r(r+1)/2, T(r,k) = C(r+k,r) * C(r,k) / (k+1)
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 146, u64 n <= 482,
 *            u128 n <= 1629, big any n
 * Helpers:   binomial, floor_sqrt, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the helper binomial(idx_t a, idx_t b) and gcd(seq_t a, seq_t b)
   from the utils library.  The calculation is performed in a way that
   avoids overflow for 64‑bit seq_t values by canceling the divisor
   (k+1) with parts of the binomials.                                        */
#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/gcd.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2   */
    seq_t temp   = (seq_t)8 * (seq_t)n + 1;       /* 8*n + 1  */
    seq_t sqrtv  = floor_sqrt(temp);              /* floor(sqrt(8n+1)) */
    idx_t r      = (idx_t)((sqrtv - 1) / 2);       /* integer division */

    /* Compute starting index of row r: start = r*(r+1)/2 */
    unsigned __int128 prod = (unsigned __int128)r * (r + 1) / 2;
    idx_t start = (idx_t)prod;

    /* Column k within the row */
    idx_t k = n - start;

    /* Compute the binomial terms */
    seq_t a_val = binomial(r + k, r);   /* C(r+k,r) */
    seq_t b_val = binomial(r, k);       /* C(r,k)   */
    seq_t denom = (seq_t)(k + 1);       /* k+1      */

    /* Reduce the fraction to avoid overflow */
    seq_t d1 = gcd(b_val, denom);
    b_val  /= d1;
    denom   /= d1;

    seq_t d2 = gcd(a_val, denom);
    a_val  /= d2;
    denom   /= d2;                      /* denom should now be 1 */

    return a_val * b_val;               /* final result */
}
