/*
 * A056051  a(n) = (n-2)! - 1 (mod n).
 * https://oeis.org/A056051
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((n-2)! - 1) mod n (signed remainder)
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t mul_mod_seq(seq_t a, idx_t b, seq_t mod) {
    /* multiply a and b, reduce modulo mod using 128‑bit intermediate */
    return (seq_t)(((__int128)a * b) % mod);
}

seq_t a(idx_t n) {
    /* a(2) is the first defined term; for n < 2 the value is irrelevant */
    if (n < 2) return 0;

    seq_t N = (seq_t)n;          /* modulus */
    seq_t res = 1 % N;           /* (n-2)! computed modulo n, start with 1 */

    for (idx_t i = 2; i <= n - 2; ++i) {
        res = mul_mod_seq(res, i, N);
    }

    return res - 1;              /* (n-2)! - 1, signed remainder in [-n+1, n-1] */
}
