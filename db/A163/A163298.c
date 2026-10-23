/*
 * A163298  Sum of divisors of n minus binary order of n.
 * https://oeis.org/A163298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = sigma(n) - ceil(log2(n)) (sigma = sum of divisors of n)
 * Range:     u8 n <= 107, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ceil(log2(n)) for unsigned 64‑bit integer n.
   For n == 1 it returns 0; otherwise it returns the number of bits
   needed to represent n‑1. */
static seq_t ceil_log2_u64(idx_t n) {
    if (n <= 1) {
        return (seq_t)0;
    }
    idx_t v = n - 1;
    int log = 0;
    while (v) {
        ++log;
        v >>= 1;
    }
    return (seq_t)log;
}

/* Sum of divisors of n, computed in linear time in sqrt(n). */
static seq_t sigma_u64(idx_t n) {
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t e = n / d;
            if (e != d) {
                sum += (seq_t)e;
            }
        }
    }
    return sum;
}

/* Sequence A163298: sum of divisors of n minus binary order of n. */
seq_t a(idx_t n) {
    seq_t sigma = sigma_u64(n);
    seq_t order = ceil_log2_u64(n);
    return sigma - order;
}
