/*
 * A126524  Number of base 29 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base-29 n-digit strings where adjacent * digits
 *            differ by at most 4. * Computed by a simple dynamic‑programming
 *            recurrence on the 29‑vertex * graph with adjacency |i‑j|≤4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* dp vectors: dp[d] = number of length‑k strings ending with digit d */
    seq_t cur[29];
    seq_t next[29];

    /* base case: for length 1 every digit is allowed */
    for (idx_t d = 0; d < 29; ++d)
        cur[d] = (seq_t)1;

    /* transition for each further position */
    for (idx_t step = 1; step < n; ++step) {
        for (idx_t j = 0; j < 29; ++j) {
            seq_t sum = (seq_t)0;
            idx_t low  = (j >= 4) ? (j - 4) : 0;
            idx_t high = (j + 4 < 29) ? (j + 4) : 28;
            for (idx_t i = low; i <= high; ++i)
                sum += cur[i];
            next[j] = sum;
        }
        for (idx_t j = 0; j < 29; ++j)
            cur[j] = next[j];
    }

    /* total number of strings of length n */
    seq_t result = (seq_t)0;
    for (idx_t d = 0; d < 29; ++d)
        result += cur[d];
    return result;
}
