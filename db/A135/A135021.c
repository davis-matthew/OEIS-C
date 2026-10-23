/*
 * A135021  Triangle read by rows: T(n,r) = number of maximum r-uniform
 *          acyclic hypergraphs of order n and size n-r+1, 1 <= r <= n+1.
 * https://oeis.org/A135021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 36, u32 n <= 67, u64 n <= 137,
 *            u128 n <= 327, big any n
 * Helpers:   big_pow, binomial, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A135021: Triangle read by rows
   T(n,r) = binomial(n,r-1) * (n*(r-1)-r^2+2*r)^(n-r-1)  (for 2 ≤ r ≤ n-1)
   T(n,1) = T(n,n) = T(n,n+1) = 1
   a(n) is the flattening of this triangle by rows.
*/

#include "../utils/big_pow.h"
#include "../utils/binomial.h"
#include "../utils/seq_one.h"

seq_t a(idx_t n)
{
    /* Find the row 'm' that contains index n.
       Row m has length m+1 and the cumulative total up to that row is
       (m+1)(m+2)/2.  We find the smallest m such that this value > n. */
    idx_t m = 0;
    while ((m + 1) * (m + 2) / 2 <= n)
        ++m;

    idx_t prev_total = m * (m + 1) / 2;  // total elements before row m
    idx_t offset = n - prev_total;      // offset within current row (0‑based)
    idx_t r = offset + 1;               // r is 1‑based in the definition

    /* Boundary cases give the value 1 */
    if (r == 1 || r == m || r == m + 1)
        return seq_one();

    /* General case: 2 ≤ r ≤ m-1 */
    seq_t bin = binomial(m, r - 1);
    seq_t base = (seq_t)m * (seq_t)(r - 1)
               - (seq_t)r * (seq_t)r
               + (seq_t)2 * (seq_t)r;
    idx_t exp = m - r - 1;               // Non‑negative here
    seq_t pow_val = big_pow(base, (seq_t)exp);
    return bin * pow_val;
}
