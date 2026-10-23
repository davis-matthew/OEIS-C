/*
 * A104325  Number of runs of equal bits in the dual Zeckendorf representation
 *          of n (A104326).
 * https://oeis.org/A104325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear, look
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Compute a(n) = number of runs of equal bits in the
   dual Zeckendorf representation of n.
   The dual Zeckendorf representation is the lexicographically
   greatest binary string (highest number of 1's) that
   corresponds to a set of distinct Fibonacci numbers summing to n.
   This is equivalent to choosing the largest possible set of
   Fibonacci numbers whose sum is n.  The function obtains this
   set by dynamic programming and then counts the runs of equal bits
   in the binary string that uses all Fibonacci numbers up to the
   largest one used.
*/

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Special case n = 0 : a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    /* Build Fibonacci numbers: 1,2,3,5,... up to ≤ n */
    /* We generate them on the fly into a small array */
    uint64_t fib[80];          /* enough for n up to 1e15 (F80 ≈ 6e16) */
    size_t k = 0;
    if (n >= 1) fib[k++] = 1;
    if (n >= 2) fib[k++] = 2;
    while (k >= 2) {
        uint64_t next = fib[k-1] + fib[k-2];
        if (next > n) break;
        fib[k++] = next;
    }

    /* dp[i*(n+1)+s] = maximum number of Fibonacci terms
       using first i numbers that sum to s   (i = 0..k)   */
    size_t rows = k + 1;
    size_t cols = (size_t)n + 1;
    int *dp = (int *)malloc(rows * cols * sizeof(int));
    if (!dp) return (seq_t)0;                 /* out of memory */

    const int NEG_INF = -1000000;            /* sentinel for impossible */
    for (size_t idx = 0; idx < rows * cols; ++idx)
        dp[idx] = NEG_INF;
    dp[0] = 0;                                /* dp[0][0] = 0 */

    /* Populate DP table */
    for (size_t i = 1; i <= k; ++i) {
        uint64_t fv = fib[i-1];
        size_t base_cur = i * cols;
        size_t base_prev = (i-1) * cols;
        for (size_t s = 0; s < cols; ++s) {
            int best = dp[base_prev + s];
            if (s >= fv) {
                int prev = dp[base_prev + (s - fv)];
                if (prev > NEG_INF) {
                    int cand = prev + 1;
                    if (cand > best) best = cand;
                }
            }
            dp[base_cur + s] = best;
        }
    }

    /* Back‑track to determine which Fibonacci numbers are used */
    char bits[80] = {0};  /* bits[i] refers to fib[i-1] */
    size_t remaining = (size_t)n;
    for (int i = (int)k; i >= 1; --i) {
        uint64_t fv = fib[i-1];
        size_t base_cur = i * cols;
        size_t base_prev = (i-1) * cols;
        if (remaining >= fv) {
            int prev = dp[base_prev + (remaining - fv)];
            if (prev > NEG_INF && prev + 1 == dp[base_cur + remaining]) {
                bits[i] = 1;
                remaining -= fv;
                continue;
            }
        }
        bits[i] = 0;
    }

    free(dp);

    /* Find highest Fibonacci index used (must be >0 because n>0) */
    size_t last = 0;
    for (size_t i = 1; i <= k; ++i)
        if (bits[i]) last = i;

    /* Count runs of equal bits in the trimmed representation
       (from the highest used bit down to the lowest Fibonacci index) */
    int runs = 1;
    char prev = bits[last];
    for (size_t i = last; i > 1; --i) {
        char cur = bits[i-1];
        if (cur != prev) {
            ++runs;
            prev = cur;
        }
    }

    return (seq_t)runs;
}
