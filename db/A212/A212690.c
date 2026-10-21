/*
 * A212690  Number of (w,x,y,z) with all terms in {1,...,n} and 2|w-x| <=
 *          n+|y-z|.
 * https://oeis.org/A212690
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A212690:
   a(n) = number of quadruples (w,x,y,z) ∈ {1,..,n}^4 with
   2|w−x| ≤ n + |y−z|.
   Closed‑form formula derived combinatorially:
   Let h   = ⌊n/2⌋,  m = ⌊(n−1)/2⌋,  N = n.
   a(n) = N² · [ N + 2hN − h(h+1) ]                // pairs with |w−x| ≤ h
          + 8·∑_{k=1}^{m} k³ + 4·∑_{k=1}^{m} k²   // pairs with |w−x| > h
*/
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    seq_t N = (seq_t)n;

    /* h = floor(n/2), m = floor((n-1)/2) */
    idx_t h = n >> 1;
    idx_t m = (n - 1) >> 1;
    seq_t H = (seq_t)h;
    seq_t M = (seq_t)m;

    /* part with small |w−x| */
    seq_t inside1 = N * ((seq_t)1 + (seq_t)2 * H) - H * (H + 1);
    seq_t sum1 = N * N * inside1;

    /* part with large |w−x| */
    /* Σ k² = M(M+1)(2M+1)/6  */
    seq_t sum_k2 = M * (M + 1) * ((seq_t)2 * M + 1) / 6;
    /* Σ k³ = [M(M+1)/2]²   */
    seq_t tmp = M * (M + 1) / 2;
    seq_t sum_k3 = tmp * tmp;

    seq_t sum2 = (seq_t)8 * sum_k3 + (seq_t)4 * sum_k2;

    return sum1 + sum2;
}
