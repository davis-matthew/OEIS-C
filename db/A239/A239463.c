/*
 * A239463  a(n) = A239460(n) / n^2.
 * https://oeis.org/A239463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 10^d + n where d = number of decimal digits of n^3
 * Range:     u8 n <= 4, u16 n <= 21, u32 n <= 999, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 958 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

static int digits_of_seq(seq_t v) {
    int d = 0;
    do {
        d++;
        v /= (seq_t)10;
    } while (v != (seq_t)0);
    return d;
}

seq_t a(idx_t n) {
    seq_t N   = (seq_t)n;
    seq_t N2  = N * N;
    seq_t N3  = N2 * N;                /* n^3 */
    int d    = digits_of_seq(N3);      /* number of decimal digits of n^3 */
    seq_t pow10_val = pow10((idx_t)d); /* 10^d  (returns seq_t) */
    return pow10_val + N;
}
