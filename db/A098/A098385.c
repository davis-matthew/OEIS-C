/*
 * A098385  Ordered factorizations over hook-type prime signatures with
 *          exactly three distinct primes (third column of A098348).
 * https://oeis.org/A098385
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2^n * (n^2 + 8*n + 13)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 22, u64 n <= 52, u128 n <= 114,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t poly = n_seq * n_seq + (seq_t)8 * n_seq + (seq_t)13;

    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow2 *= (seq_t)2;
    }

    return pow2 * poly;
}
