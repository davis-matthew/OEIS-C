/*
 * A182112  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and (w+n)^2=x+y+w.
 * https://oeis.org/A182112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y∈[-n,n]
 *            satisfying (w+n)^2 = x + y + w. Computation: for each integer w
 *            in [-n,n] compute S = w^2 + (2n-1)w + n^2. Count the number of
 *            pairs (x,y) in [-n,n] with sum S, which is max(0, 2n+1 - |S|) if
 *            |S| ≤ 2n, otherwise 0. Return the sum of those...
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Use a signed 64‑bit variable for calculations; n is at most 10000
       (the benchmark data) so 64‑bit holds all intermediate values safely. */
    int64_t N = (int64_t)n;              /* n as signed 64‑bit */
    int64_t twoN      = 2 * N;
    int64_t twoN_plus = twoN + 1;
    int64_t coeff = 2 * N - 1;           /* 2n-1 */
    int64_t NN = N * N;                  /* n^2 */

    int64_t total = 0;
    for (int64_t w = -N; w <= N; ++w) {
        int64_t S = w * w + coeff * w + NN;   /* (w+n)^2 - w  */
        int64_t absS = S >= 0 ? S : -S;
        if (absS <= twoN) {
            total += (twoN_plus - absS);
        }
    }
    return (seq_t)total;
}
