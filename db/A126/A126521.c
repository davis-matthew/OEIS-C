/*
 * A126521  Number of base 26 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1. For n >= 1, let f(1, d) = 1 for all 0 <= d < 26. For m
 *            > 1, f(m, d) = sum_{k: |k-d| <= 4} f(m-1, k). Then a(n) =
 *            sum_{i=0}^{25} f(n, i)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* DP rows for the previous and current length */
    static seq_t prev[26];
    static seq_t curr[26];
    /* Initialize f(1, d) = 1 */
    for (idx_t d = 0; d < 26; ++d)
        prev[d] = 1;

    /* Build up to length n */
    for (idx_t len = 2; len <= n; ++len) {
        for (idx_t d = 0; d < 26; ++d) {
            idx_t low  = d < 4 ? 0 : d - 4;
            idx_t high = d > 21 ? 25 : d + 4;
            seq_t sum = 0;
            for (idx_t k = low; k <= high; ++k)
                sum += prev[k];
            curr[d] = sum;
        }
        /* copy curr to prev for next iteration */
        for (idx_t d = 0; d < 26; ++d)
            prev[d] = curr[d];
    }

    /* Sum all ending digits to obtain a(n) */
    seq_t total = 0;
    for (idx_t d = 0; d < 26; ++d)
        total += prev[d];
    return total;
}
