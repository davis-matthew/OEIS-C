/*
 * A085884  Let r and s be such that r + s = n; a(n) = maximum value of
 *          sigma(r) + sigma(s).
 * https://oeis.org/A085884
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1<=r<n} ( sigma(r) + sigma(n-r) ) where sigma(m) is
 *            the sum of all positive divisors of m
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes sigma for all numbers 1..n by a divisor‑sum sieve
   and then scans the n-1 possibilities to find the maximum sum. */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n) {
    if (n < 2) return 0;                     /* indices below the offset are irrelevant */

    /* Allocate a temporary array to hold sigma(1..n).  All entries are initialized to 0. */
    seq_t *sigma = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!sigma) return 0;                     /* allocation failure */
    for (idx_t i = 0; i <= n; ++i) sigma[i] = 0;

    /* Divisor‑sum sieve: for every divisor i add i to all multiples j. */
    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t j = i; j <= n; j += i) {
            sigma[j] += (seq_t)i;
        }
    }

    /* Scan all partitions r + s = n, r>=1, s>=1, to find the maximum sigma(r)+sigma(s). */
    seq_t best = 0;
    for (idx_t r = 1; r < n; ++r) {
        seq_t sum = sigma[r] + sigma[n - r];
        if (sum > best) best = sum;
    }

    free(sigma);
    return best;
}
