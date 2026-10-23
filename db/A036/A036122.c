/*
 * A036122  a(n) = 2^n mod 29.
 * https://oeis.org/A036122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 29
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t if needed */

/* Compute a(n) = 2^n mod 29.  Works for any idx_t n >= 0. */
seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)29;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    idx_t exp    = n;

    while (exp) {
        if (exp & ((idx_t)1)) {
            result = (result * base) % MOD;
        }
        base = (base * base) % MOD;
        exp >>= 1;
    }

    return result;
}
