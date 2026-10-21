/*
 * A211922  Number of (w,x,y,z) with all terms in {1,...,n} and 2*w*x>3*y*z.
 * https://oeis.org/A211922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) ∈ {1,…,n}^4 with 2*w*x >
 *            3*y*z. Computation is performed in O(n²) time and O(n²) memory
 *            using the following identity: a(n) = ∑_{A=1}^{n²} f(A) * F(
 *            ⌊(2A‑1)/3⌋ ) where f(A) = number of ordered pairs (w,x) with w·x
 *            = A F(B) = ∑_{t=1}^{B} f(t). The prefix sums F...
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* External integer types supplied by the build environment. */
extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* n is unsigned 64‑bit; for n=0 we return 0 immediately. */
    if (n == 0)
        return (seq_t)0;

    /* Size of the product array: maximum product is n*n. */
    idx_t Nsq = n * n;          /* up to 1e8 for n <= 10000 */
    size_t arrSize = (size_t)Nsq + 1;   /* include index 0 */

    /* Allocate array f[0 … n²] as uint16_t (counts <= n <= 10000). */
    uint16_t *f = (uint16_t *)calloc(arrSize, sizeof(uint16_t));
    if (!f)
        return (seq_t)0;        /* allocation failed – return 0 */

    /* Count product multiplicities f(A).  */
    for (idx_t w = 1; w <= n; ++w)
    {
        for (idx_t x = 1; x <= n; ++x)
        {
            size_t prod = (size_t)w * (size_t)x;   /* prod ∈ [1, n²] */
            ++f[prod];
        }
    }

    /* Main summation: scan A from 1 to n².  */
    idx_t curR = 0;          /* current R value being accumulated */
    uint64_t sumR = 0;       /* prefix sum f[1] + … + f[curR]   */
    seq_t answer = 0;        /* final result                    */

    for (idx_t A = 1; A <= Nsq; ++A)
    {
        /* R = floor((2*A - 1) / 3)  – target prefix index for this A. */
        idx_t R = (2 * A - 1) / 3;

        /* Advance curR up to R, accumulating f values. */
        while (curR < R)
        {
            ++curR;
            sumR += f[curR];
        }

        /* Add contribution of current A: f[A] * sum_{t≤R} f[t]. */
        answer += (seq_t)f[A] * ((seq_t)sumR);
    }

    free(f);
    return answer;
}
