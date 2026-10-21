/*
 * A078267  Smallest k such that k*N is an integer where N is obtained by
 *          placing the string "n" after a decimal point.
 * https://oeis.org/A078267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, frac, nonn
 * Formula:   a(n) = 10^digits(n) / gcd(n, 10^digits(n))
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A078267 – smallest k such that k * 0.n is an integer */

#include <stdint.h>
#include "../utils/_digit_count.h"

/* Euclidean algorithm for seq_t (works for all supported integer types). */
static inline seq_t _gcd_seq_local(seq_t a, seq_t b)
{
    while (b != 0) {
        seq_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* Convert n to seq_t once. */
    seq_t n_seq = (seq_t)n;

    /* Number of decimal digits of n. */
    idx_t d = _digit_count(n);

    /* Compute 10^d in seq_t. */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < d; ++i)
        pow10 *= 10;

    /* Compute gcd(10^d, n). */
    seq_t g = _gcd_seq_local(pow10, n_seq);

    /* Return 10^d / gcd.  The division is exact. */
    return pow10 / g;
}
