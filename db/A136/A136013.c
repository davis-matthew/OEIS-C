/*
 * A136013  a(n) = floor(n/2) + 2*a(floor(n/2)), a(0) = 0.
 * https://oeis.org/A136013
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + 2*a(floor(n/2)), a(0) = 0
 * Range:     u8 n <= 95, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Equivalent bitwise computation: a(n) = Σ i*b_i*2^(i-1) */

seq_t a(idx_t n)
{
    seq_t res = 0;
    idx_t i = 0;
    idx_t tmp = n;
    while (tmp) {
        if (tmp & 1ULL) {
            if (i > 0) {
                res += (seq_t)i * ((seq_t)(1ULL << (i - 1)));
            }
        }
        tmp >>= 1;
        ++i;
    }
    return res;
}
