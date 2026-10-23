/*
 * A021551  Decimal expansion of 1/547.
 * https://oeis.org/A021551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = floor(10^(n+1)/547) mod 10 Equivalent to: a(n) = ((10^n
 *            mod 547) * 10) / 547 for n >= 0 The function returns the nth
 *            decimal digit of 1/547, with a(0) the first digit after the
 *            decimal point
 * Range:     u8 n <= 0, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t modpow(seq_t base, idx_t exp, seq_t mod) {
    seq_t result = (seq_t)1;
    seq_t b = base % mod;
    while (exp > (idx_t)0) {
        if (exp & (idx_t)1) {
            result = (result * b) % mod;
        }
        b = (b * b) % mod;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n) {
    /* Compute 10^n mod 547 */
    seq_t r = modpow((seq_t)10, n, (seq_t)547);
    /* Digit is floor(10*r / 547) which is in [0,9] */
    seq_t digit = (r * (seq_t)10) / (seq_t)547;
    return digit;
}
