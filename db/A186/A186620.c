/*
 * A186620  Fibonacci sequence beginning 12, 25.
 * https://oeis.org/A186620
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0)=12, a(1)=25
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)12;
    if (n == 1) return (seq_t)25;

    seq_t prev = (seq_t)12;
    seq_t curr = (seq_t)25;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t next = prev + curr;
        prev = curr;
        curr = next;
    }
    return curr;
}
