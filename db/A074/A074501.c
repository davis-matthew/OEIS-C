/*
 * A074501  a(n) = 1^n + 2^n + 5^n.
 * https://oeis.org/A074501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 2^n + 5^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute base^exp for unsigned integer types seq_t and idx_t. */
static inline seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1)
            result *= cur;
        /* Avoid an unnecessary multiplication when exp==1 */
        if (exp > 1)
            cur *= cur;
        exp >>= 1;
    }
    return result;
}

/* OEIS A074501: a(n) = 1^n + 2^n + 5^n */
seq_t a(idx_t n)
{
    seq_t pow2 = pow_uint(2, n);
    seq_t pow5 = pow_uint(5, n);
    seq_t result = 1;
    result += pow2;
    result += pow5;
    return result;
}
