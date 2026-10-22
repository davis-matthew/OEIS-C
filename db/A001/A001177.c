/*
 * A001177  Fibonacci entry points: a(n) = least k >= 1 such that n divides
 *          Fibonacci number F_k (=A000045(k)).
 * https://oeis.org/A001177
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search: a(n) = least k >= 1 with F(k) == 0 (mod n), found by
 *            iterating Fibonacci numbers mod n (a(n) <= 2n)
 * Range:     u8 n <= 149, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    uint64_t m = (uint64_t)n;
    uint64_t f0 = 0, f1 = 1; /* F(k-1), F(k) mod m, starting at k = 1 */
    uint64_t k = 1;
    while (f1 != 0) {
        uint64_t f2 = f0 + f1;
        if (f2 >= m) f2 -= m;
        f0 = f1;
        f1 = f2;
        k++;
    }
    return (seq_t)k;
}
