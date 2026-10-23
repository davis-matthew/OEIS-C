/*
 * A037845  a(n) = Sum_{i=1..m, d(i)<d(i-1)} d(i-1)-d(i), where Sum_{i=0..m}
 *          d(i)*4^i is the base 4 representation of n.
 * https://oeis.org/A037845
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_{i=1..m, d(i) < d(i-1)} (d(i-1) - d(i)), where n =
 *            Σ_{i=0..m} d(i)·4^i is the base‑4 expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* The sequence is defined for n ≥ 1 (offset 1).  For safety,
       return 0 for n == 0 which should never be requested. */
    if (n == 0)
        return 0;

    idx_t prev = n & 3;          /* d(0) – least significant base‑4 digit */
    idx_t rest = n >> 2;         /* remove the least significant digit */
    seq_t sum = 0;

    while (rest) {
        idx_t cur = rest & 3;    /* next digit d(i) */
        if (cur < prev) {
            sum += (seq_t)(prev - cur);
        }
        prev = cur;
        rest >>= 2;              /* move to the next digit */
    }

    return sum;
}
