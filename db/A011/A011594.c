/*
 * A011594  Legendre symbol (n,59).
 * https://oeis.org/A011594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = n^29 (mod 59) – Legendre symbol (n,59) (0 if 59 divides
 *            n, 1 if n is a quadratic residue mod 59, -1 otherwise)
 * Range:     i8 n <= 138, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    const seq_t MOD = (seq_t)59;
    seq_t base = (seq_t)n % MOD;
    if (base == (seq_t)0) {
        return (seq_t)0;
    }
    seq_t result = (seq_t)1;
    seq_t exp = (seq_t)29;
    while (exp) {
        if (exp & (seq_t)1) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }
    if (result == MOD - (seq_t)1) {
        return (seq_t)(-1);
    }
    return result;   /* should be 1 */
}
