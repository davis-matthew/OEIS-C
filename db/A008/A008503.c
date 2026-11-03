/*
 * A008503  9-dimensional centered tetrahedral numbers.
 * https://oeis.org/A008503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binom(n+10,10) - binom(n,10)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 36, u64 n <= 443, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 871 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* binom(n,10) = product_{i=1}^{10} (n-10+i)/i  (exact integer division) */

#include <stdint.h>

static seq_t binom_n10(idx_t n) {
    if (n < 10) return 0;
    seq_t res = 1;
    for (idx_t i = 1; i <= 10; ++i) {
        /* use 128‑bit intermediate to avoid overflow before division */
        __uint128_t tmp = ( __uint128_t )res * (n - 10 + i);
        tmp /= i;
        res = (seq_t)tmp;
    }
    return res;
}

seq_t a(idx_t n) {
    seq_t t1 = binom_n10(n + 10);
    seq_t t2 = binom_n10(n);
    return t1 - t2;
}
