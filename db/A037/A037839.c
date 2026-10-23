/*
 * A037839  a(n) = Sum{|d(i)-d(i-1)|: i=1,...,m}, where Sum{d(i)*7^i:
 *          i=0,1,...,m} is the base 7 representation of n.
 * https://oeis.org/A037839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=1}^{m} |d(i)-d(i-1)| where Sum_{i=0}^{m} d(i)*7^i
 *            = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = 0;
    int prev = -1;            // sentinel for first digit
    while (n > 0) {
        int cur = (int)(n % 7);   // current least‑significant digit
        if (prev != -1) {
            int diff = cur - prev;
            if (diff < 0) diff = -diff;
            res += (seq_t)diff;
        }
        prev = cur;
        n /= 7;
    }
    return res;
}
