/*
 * A169651  a(0)=a(1)=1, a(2)=2; thereafter a(n) = 3*a(n-1) + 3*2^(n-3) - 2.
 * https://oeis.org/A169651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*3^(n-2) - 3*2^(n-2) + 1 (n>=2); a(0)=a(1)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t pow_u(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & (idx_t)1) result *= base;
        base *= base;
        exp >>= (idx_t)1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0u || n == 1u)
        return (seq_t)1;
    /* n >= 2 */
    idx_t exp = n - 2u;
    seq_t pow3 = pow_u((seq_t)3, exp);
    seq_t pow2 = pow_u((seq_t)2, exp);
    seq_t value = (seq_t)4 * pow3;
    value -= (seq_t)3 * pow2;
    value += (seq_t)1;
    return value;
}
