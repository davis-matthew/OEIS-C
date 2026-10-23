/*
 * A132789  Triangle read by rows: T(n,k) = A007318(n-1, k-1) + A001263(n, k)
 *          - 1.
 * https://oeis.org/A132789
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 82, u32 n <= 219, u64 n <= 720,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A132789
   T(n,k) = binomial(n-1,k-1)*(1 + binomial(n,k-1)/k) - 1
   a(index) = T(r,c) where r = smallest integer with r*(r+1)/2 >= index
   and c = index - r*(r-1)/2 (1‑based column) */
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* Find triangle row r and column c for index n (n>=1) */
    idx_t r = 1;
    idx_t tri = 1;           /* cumulative number of terms up to row r */
    while (tri < n) {
        ++r;
        tri += r;            /* tri = r*(r+1)/2 */
    }
    /* Column number (1‑based) within the row */
    idx_t prev_tri = tri - r;            /* (r-1)*r/2 */
    idx_t c = n - prev_tri;              /* 1 ≤ c ≤ r */

    /* Compute T(r,c) = (C(r-1,c-1)*(c + C(r,c-1)))/c - 1   */
    seq_t b1 = binomial(r - 1, c - 1);    /* C(r-1,c-1)   */
    seq_t b2 = binomial(r,     c - 1);    /* C(r,c-1)     */
    seq_t k = (seq_t)c;                   /* column as seq_t */

    /* Avoid overflow by dividing before multiplying:
       let q = b1 / k,  r = b1 % k  (both integers)
       then (b1*(k+b2)) / k = q*(k+b2) + (r*(k+b2))/k   */
    seq_t sum = k + b2;                    /* (k + b2) */
    seq_t q = b1 / k;
    seq_t rem = b1 % k;
    seq_t part1 = q * sum;
    seq_t part2 = (rem * sum) / k;         /* rem < k ensures integer division */
    seq_t res = part1 + part2 - (seq_t)1;   /* subtract 1 at end */

    return res;
}
