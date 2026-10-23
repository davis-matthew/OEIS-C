/*
 * A037833  Number of i such that d(i) >= d(i-1), where Sum_{i=0..m} d(i)*10^i
 *          is the base-10 representation of n.
 * https://oeis.org/A037833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = number of indices i>=1 with d(i) >= d(i-1) where n = Σ
 *            d(i)·10^i (decimal digits d(0) is least significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t count = 0;
    idx_t temp = n;
    bool first = true;
    int prev = 0;

    while (temp > 0) {
        int cur = temp % 10;       /* current higher place digit   */
        temp /= 10;                /* remove that digit           */

        if (!first) {
            if (cur >= prev) count++;   /* compare with lower digit   */
        }
        prev = cur;
        first = false;
    }
    return count;
}
