/*
 * A216130  7^n mod 10000.
 * https://oeis.org/A216130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 10000
 * Range:     u8 n <= 1, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

const seq_t MOD = 10000;

/* Computes a(n) = 7^n mod 10000, with 1-indexed offset (a(1)=7) */
seq_t a(idx_t n) {
    seq_t result = 1 % MOD;
    seq_t base   = 7 % MOD;
    idx_t e = n;
    while (e > 0) {
        if (e & 1u)
            result = (seq_t)((seq_t)result * base % MOD);
        base = (seq_t)((seq_t)base * base % MOD);
        e >>= 1;
    }
    return result;
}
