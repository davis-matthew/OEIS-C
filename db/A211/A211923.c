/*
 * A211923  Number of (w,x,y,z) with all terms in {1,...,n} and 2*w*x>=3*y*z.
 * https://oeis.org/A211923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of quadruples (w,x,y,z) with 1≤w,x,y,z≤n and 2*w*x
 *            >= 3*y*z
 * Range:     u8 n <= 5, u16 n <= 20, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 by definition */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Maximum product of two numbers from 1..n. */
    idx_t n2 = n * n;

    /* We only need counts of products y*z ≤ floor(2*n²/3). */
    idx_t limit = (2 * n2) / 3;           /* floor(2/3 · n²) */

    /* Allocate an array to hold frequencies of products of (y,z). */
    /* Use uint32_t because each count ≤ n (≤ 10000) and cumulative
       sum of all counts ≤ n² ≤ 1e8 which fits in 32 bits. */
    uint32_t *freq = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (!freq) {
        return (seq_t)0;                 /* allocation failed */
    }

    /* Count occurrences of each product y*z that does not exceed the limit. */
    for (idx_t y = 1; y <= n; ++y) {
        for (idx_t z = 1; z <= n; ++z) {
            idx_t prod = y * z;
            if (prod <= limit) {
                freq[prod] += 1;
            }
        }
    }

    /* Convert frequency array into prefix sums:
       freq[t] will hold the number of pairs (y,z) with product ≤ t. */
    for (idx_t i = 1; i <= limit; ++i) {
        freq[i] += freq[i - 1];
    }

    /* Now compute the answer by iterating over all pairs (w,x). */
    seq_t ans = (seq_t)0;
    for (idx_t w = 1; w <= n; ++w) {
        for (idx_t x = 1; x <= n; ++x) {
            idx_t p = w * x;                 /* product w*x */
            idx_t T = (2 * p) / 3;           /* floor(2*w*x/3) */
            ans += (seq_t)freq[T];
        }
    }

    free(freq);
    return ans;
}
