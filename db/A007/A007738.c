/*
 * A007738  Period of repeating digits of 1/n in base 7.
 * https://oeis.org/A007738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A002322-like multiplicative order: a(n) = ord_m(7) where
 *            m = n with all factors 7 removed; a(n) = 1 if m = 1
 * Range:     u8 n <= 256, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    while (m % 7 == 0) m /= 7;
    if (m == 1) return (seq_t)1;
    uint64_t order = 1;
    uint64_t power = 7 % m;
    while (power != 1) {
        power = (uint64_t)(((unsigned __int128)power * 7) % m);
        order++;
    }
    return (seq_t)order;
}
