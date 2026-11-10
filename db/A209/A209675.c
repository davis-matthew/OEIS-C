/*
 * A209675  Radon function at even positions: a(n) = A003484(2*n).
 * https://oeis.org/A209675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A003484(2*n) = 8*b + 2^c where 2*n = 2^(4*b+c)*d,
 *            0<=c<=3, d odd
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 101 terms through n = 8489 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    unsigned v = 1; /* 2-adic valuation of 2*n */
    idx_t m = n;
    while ((m & 1) == 0) {
        v++;
        m >>= 1;
    }
    unsigned b = v / 4;
    unsigned c = v % 4;
    return (seq_t)8 * (seq_t)b + ((seq_t)1 << c);
}
