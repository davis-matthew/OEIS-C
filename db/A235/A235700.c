/*
 * A235700  a(n+1) = a(n) + (a(n) mod 5), a(1)=1.
 * https://oeis.org/A235700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1 mod 4) + 10*floor((n-1)/4)
 * Range:     u8 n <= 103, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t k = n - 1;                /* k = n-1 */
    seq_t d = (seq_t)(k >> 2);      /* floor((n-1)/4) */
    uint8_t r = (uint8_t)(k & 3);   /* (n-1) mod 4 */
    seq_t part = (seq_t)1 << r;     /* 2^( (n-1) mod 4 ) */
    seq_t res  = part + (seq_t)10 * d;
    return res;
}
