/*
 * A013625  Triangle of coefficients in expansion of (4+7x)^n.
 * https://oeis.org/A013625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy
 * Formula:   a(n) is the n‑th entry in the triangle of coefficients of
 *            (4+7x)^n, read row‑by‑row: T(n,k) = binomial(n,k)*7^k*4^(n-k), 0
 *            ≤ k ≤ n. a(n) = T(r,c) where r is the smallest integer with
 *            r*(r+1)/2 > n and c = n - r*(r+1)/2
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the row r containing index n.  The cumulative number of
       entries up to row r−1 is r*(r+1)/2. */
    idx_t r = 0;
    idx_t cum = 0;                     /* cumulative count up to the previous row   */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }

    /* Position of the term inside row r */
    idx_t k = n - cum;                 /* 0‑based column index in row r              */

    /* Compute binomial coefficient, powers, and product */
    seq_t bin = binomial(r, k);                 /* C(r,k)                            */
    seq_t p7  = pow_uint(7, k);                 /* 7^k                               */
    seq_t p4  = pow_uint(4, r - k);             /* 4^(r-k)                           */
    seq_t term = seq_mul(seq_mul(bin, p7), p4);  /* bin * 7^k * 4^(r-k)                */

    return term;
}
