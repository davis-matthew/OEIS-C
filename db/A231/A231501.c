/*
 * A231501  a(n) = Sum_{i=0..n} wt(i)^3, where wt() = A000120().
 * https://oeis.org/A231501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{i=0..n} popcount(i)^3 (where popcount(i) is the
 *            number of 1-bits in i's binary representation)
 * Range:     u8 n <= 18, u16 n <= 563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t i = 0;
    for (;;) {
        /* __builtin_popcountll works on unsigned long long */
        unsigned k = __builtin_popcountll((uint64_t)i);
        seq_t cube = (seq_t)(k * k * k);
        sum += cube;
        if (i == n) break;
        ++i;
    }
    return sum;
}
