/*
 * A037912  Number of i such that |d(i) - d(i-1)| = 1, where Sum_{i=0..m}
 *          d(i)*9^i is the base-9 representation of n.
 * https://oeis.org/A037912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = #{ i ≥ 1 : |d_i - d_{i-1}| = 1 }, where n = Σ d_i * 9^i
 *            is the base‑9 representation of n (digits d_0 is least
 *            significant)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;  /* no adjacent pairs for n = 0 or 1 */
    idx_t temp = n;
    seq_t count = 0;
    uint8_t prev = 0;
    bool first = true;

    while (temp > 0) {
        uint8_t cur = (uint8_t)(temp % (idx_t)9);
        if (!first) {
            uint8_t diff = (prev > cur) ? (prev - cur) : (cur - prev);
            if (diff == 1) count++;
        }
        prev = cur;
        first = false;
        temp /= (idx_t)9;
    }
    return count;
}
