/*
 * A180883  Number of permutations p() of 1..n+6 with centered difference
 *          p(i+1)-p(i-1) < 0 exactly 5 times.
 * https://oeis.org/A180883
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(n+6, ceil((n+6)/2)) * Σ_{d=0}^{5}
 *            Eulerian(Ceil((n+6)/2), d) * Eulerian(Floor((n+6)/2), 5-d)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 40,
 *            big any n
 * Helpers:   euler
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Eulerian(m,k) = number of permutations of {1..m} with exactly k
   descents (i.e., inversions of consecutive elements). */

#include <stdbool.h>
#include "../utils/euler.h"

static seq_t
binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (seq_t)(n - k + i) / (seq_t)i;
    }
    return res;
}

seq_t
a(idx_t n)
{
    if (n < 1) return 0;

    idx_t N = n + 6;
    idx_t O = (N + 1) / 2;   /* number of odd positions */
    idx_t E = N / 2;         /* number of even positions */

    seq_t bin = binom(N, O);

    seq_t sum = 0;
    for (unsigned d = 0; d <= 5; ++d)
    {
        if (d > O) continue;
        unsigned e = 5 - d;
        if (e > E) continue;
        seq_t a1 = euler(O, d);
        seq_t a2 = euler(E, e);
        sum += a1 * a2;
    }

    return bin * sum;
}
