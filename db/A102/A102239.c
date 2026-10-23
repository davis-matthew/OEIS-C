/*
 * A102239  a(n) = (Sum_{i=0..n} 5^i) + 1 - (Sum_{i=0..n} 5^i) mod 2.
 * https://oeis.org/A102239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=0..n}5^i + 1 - (Sum_{i=0..n}5^i mod 2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   pow5_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow5_big.h"

seq_t a(idx_t n) {
    idx_t exp  = n + (idx_t)1;                    /* exponent n+1 */
    seq_t pow5 = pow5_big(exp);                   /* 5^(n+1) */
    seq_t sum  = (pow5 - (seq_t)1) / (seq_t)4;    /* (5^(n+1)-1)/4 */
    seq_t parity = sum & (seq_t)1;                /* sum mod 2 */
    return sum + (seq_t)1 - parity;
}
