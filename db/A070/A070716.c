/*
 * A070716  n^7 mod 35.
 * https://oeis.org/A070716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 35
 * Range:     u8 n <= 76, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t mod = (seq_t)35;        /* modulus as seq_t */
    seq_t base = (seq_t)n % mod;  /* reduce n modulo 35 first */
    seq_t res  = (seq_t)1;        /* accumulate result */

    /* exponentiation by repeated multiplication (7 times) */
    for (idx_t i = 0; i < 7; ++i) {
        res = (res * base) % mod;
    }
    return res;
}
