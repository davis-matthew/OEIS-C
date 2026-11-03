/*
 * A118977  a(0)=0, a(1)=1; a(2^i+j) = a(j) + a(j+1) for 0 <= j < 2^i.
 * https://oeis.org/A118977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; a(1)=1; a(2^i + j)=a(j)+a(j+1) for 0 <= j < 2^i
 * Range:     u8 n <= 1017, u16 n <= 9999, u32 n <= 9999, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Validated: standard, pass, 111 terms through n = 9560 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Recursively computes the highest power of two not exceeding n. */
static inline idx_t high_power_of_two(idx_t n) {
    idx_t p = 1;
    /* p<<1 may overflow only when it exceeds n, which is fine here. */
    while ((p << 1) <= n) {
        p <<= 1;
    }
    return p;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    idx_t p = high_power_of_two(n);
    idx_t j = n - p;
    /* Recursive definition: a(n) = a(j) + a(j+1) */
    return a(j) + a(j + 1);
}
