/*
 * A033455  Convolution of nonzero squares A000290 with themselves.
 * https://oeis.org/A033455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n+1)*(((n+1)^4)-1))/30 (OEIS A033455)
 * Range:     u8 n <= 4, u16 n <= 17, u32 n <= 165, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* shift index because OEIS formula uses a(n‑1) = n*(n⁴‑1)/30   */
    idx_t nn = n + 1;                     /* integer type, no overflow for idx_t */
    /* use 128‑bit arithmetic for intermediate values to avoid overflow */
    __uint128_t big_n  = nn;               /* promote to 128 bit */
    __uint128_t n2     = big_n * big_n;    /* n² */
    __uint128_t n4     = n2 * n2;          /* n⁴  */
    __uint128_t num    = big_n * (n4 - 1); /* n*(n⁴‑1) */
    __uint128_t res    = num / 30;         /* exact division by 30 */
    return (seq_t)res;                     /* cast back to sequence type */
}
