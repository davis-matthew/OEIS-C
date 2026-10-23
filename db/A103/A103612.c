/*
 * A103612  Number of solutions to 5+B^2=p^2+q^2 with B=2n, p,q>0 and
 *          2p^2<5+B^2.
 * https://oeis.org/A103612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A103612: the number of solutions to 5+B^2 = p^2+q^2
   with B = 2n, p≥0, q>p, and 2p^2 < 5+B^2.
   For each n ≥ 0, let T = 5 + 4n^2; then count integer pairs
   (p,q) with p^2+q^2 = T and 2p^2 < T.  This is equivalent to
   counting representations of T as a sum of two squares with
   p < q, allowing p=0 when it satisfies the inequality. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root for uint64_t: floor(sqrt(x)) */
static inline uint64_t isqrt_u64(uint64_t x)
{
    uint64_t lo = 0, hi = 1ULL << 32;   /* upper bound > sqrt(2^64-1) */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* Convert n once to 64‑bit to avoid repeated static casts. */
    uint64_t nn = (uint64_t)n;

    /* T = 5 + 4n^2, fits in 64 bits for n ≤ 10000. */
    uint64_t T = 5ULL + 4ULL * nn * nn;

    /* Upper bound for p: floor(sqrt((T-1)/2)).  The +1 on the
       denominator ensures that 2p^2 < T. */
    uint64_t bound = isqrt_u64((T - 1) / 2);

    seq_t count = 0;

    for (uint64_t p = 0; p <= bound; ++p) {
        uint64_t p2 = p * p;
        uint64_t q2 = T - p2;
        uint64_t q  = isqrt_u64(q2);
        if (q * q == q2) {
            /* Since 2p^2 < T, we automatically have q > p. */
            count++;
        }
    }

    return count;
}
