/*
 * A211423  Number of ordered triples (w,x,y) with all terms in
 *          {-n,...,0,...,n} and w^2 + 2*x*y = 0.
 * https://oeis.org/A211423
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ordered triples (w,x,y) with w,x,y ∈ {‑n,…,n}
 *            and w² + 2xy = 0. The value is a(n) = 4n+1 + 4·∑_{k=1}^{⌊n/2⌋}
 *            Q(k,n) where Q(k,n) = number of positive divisors d of 2k²
 *            satisfying d≤n and (2k²)/d ≤ n
 * Range:     u8 n <= 23, u16 n <= 3107, u32 n <= 9512, u64 n <= 9512,
 *            u128 n <= 9512, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* Returns a(n) as defined above.  n ≥ 0. */
seq_t a(idx_t n)
{
    /* w = 0 contributes 5 solutions for any n ≥ 1, plus 1 when n = 0. */
    seq_t result = (seq_t)4 * (seq_t)n + (seq_t)1;

    if (n < 2)               /* no even w ≠ 0 */
        return result;

    idx_t kmax = n / 2;      /* largest k with w = ±2k in range */

    for (idx_t k = 1; k <= kmax; ++k) {
        /* N = 2·k².  The product xy must equal –N. */
        uint64_t N = (uint64_t)2 * (uint64_t)k * (uint64_t)k;

        /* integer square root of N */
        idx_t sqrtN = (idx_t)sqrt((double)N);

        idx_t count_in_range = 0;

        /* Count divisor pairs {d, N/d} with both ≤ n. */
        for (idx_t d = 1; d <= sqrtN; ++d) {
            if (N % d != 0)
                continue;

            idx_t d2 = N / d;
            if (d > n || d2 > n)
                continue;

            if (d == d2)
                count_in_range += 1;   /* perfect square case */
            else
                count_in_range += 2;   /* two distinct divisors */
        }

        /* Each qualifying divisor contributes 4 triples (two w values, two
           sign choices for the divisor → two ordered pairs of (x,y)). */
        result += (seq_t)4 * (seq_t)count_in_range;
    }

    return result;
}
