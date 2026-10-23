/*
 * A174935  a(n) = Sum_{k<=n} A007955(k) * A000027(k) = Sum_{k<=n} A007955(k)
 *          * k, where A007955(m) = product of divisors of m.
 * https://oeis.org/A174935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (product of divisors of k) * k Product of
 *            divisors of k = k^(d(k)/2), where d(k) = number of divisors of
 *            k. When d(k) is odd (k is a square) the exponent is
 *            half‑integer, so we compute (k^((d-1)/2)) * sqrt(k)
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   seq_pow_native
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/seq_pow_native.h"

/* Integer square root (floor) for seq_t using binary search. */
static seq_t int_floor_sqrt(seq_t n)
{
    seq_t low = 0;
    seq_t high = n;
    while (low <= high) {
        seq_t mid = (low + high) >> 1;
        seq_t midsq = mid * mid;
        if (midsq == n) return mid;
        if (midsq < n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* floor(sqrt(n)) */
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for offset, ignore */

    /* Allocate array for divisor counts (tau). */
    size_t N = (size_t)n;          /* N fits into size_t (n ≤ 10000 in tests) */
    idx_t *tau = (idx_t *)malloc((N + 1) * sizeof(idx_t));
    if (!tau) return 0;            /* allocation failure */

    /* Initialize divisor counts to 0. */
    for (size_t i = 0; i <= N; ++i)
        tau[i] = 0;

    /* Simple O(N log N) sieve to count divisors. */
    for (size_t i = 1; i <= N; ++i) {
        for (size_t j = i; j <= N; j += i)
            ++tau[j];
    }

    /* Compute the cumulative sum. */
    seq_t sum = 0;
    for (size_t k = 1; k <= N; ++k) {
        idx_t d = tau[k];
        seq_t k_seq = (seq_t)k;
        seq_t prod;

        if ((d & 1) == 0) {                         /* d even */
            idx_t exp = d / 2;
            prod = seq_pow_native(k_seq, exp);
        } else {                                     /* d odd -> k is a perfect square */
            seq_t sq = int_floor_sqrt(k_seq);
            idx_t exp = (d - 1) / 2;
            prod = seq_pow_native(k_seq, exp) * sq;
        }

        sum += prod * k_seq;
    }

    free(tau);
    return sum;
}
