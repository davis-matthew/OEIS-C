/*
 * A139243  Second differences of Mersenne numbers A001348, divided by 4.
 * https://oeis.org/A139243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^(p_{n+2}) - 2*2^(p_{n+1}) + 2^(p_n)) / 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 29,
 *            big any n
 * Helpers:   get_prime, mul_small, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow_uint.h"
#include "../utils/mul_small.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    /* prime indices: first prime = 2 */
    idx_t pn  = (idx_t)get_prime(n);
    idx_t pn1 = (idx_t)get_prime(n + 1);
    idx_t pn2 = (idx_t)get_prime(n + 2);

    idx_t d1 = pn1 - pn;
    idx_t d2 = pn2 - pn1;

    /* small factor S = 2^(d1+d2) - 2*2^d1 + 1 */
    seq_t pow_d1d2 = pow_uint(2, d1 + d2);
    seq_t pow_d1   = pow_uint(2, d1);
    seq_t temp     = mul_small(pow_d1, 2);          /* 2 * 2^d1 */
    seq_t S = pow_d1d2 - temp + 1;                 /* subtraction then addition */

    /* base = 2^(p_n - 2) */
    seq_t base = pow_uint(2, pn - 2);

    /* result = base * S */
    return seq_mul(base, S);
}
