/*
 * A222892  Number of n X 3 0..7 arrays with no element equal to another at a
 *          city block distance of exactly two, and new values 0..7 introduced
 *          in row major order.
 * https://oeis.org/A222892
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^n + 5*2^(n-1)*7^(n-2) + 14*5^(n-3)*31^(n-2) +
 *            10*39^(n-2) for n > 2 a(1) = 3; a(2) = 34
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include the multiplication helper for seq_t. */
#include "../utils/seq_mul.h"

/* Fast exponentiation for seq_t.  Compiles as valid C11 and C++17. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1)
            result = seq_mul(result, cur);
        cur = seq_mul(cur, cur);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    if (n == 2)
        return (seq_t)34;

    /* n >= 3 */
    seq_t term1 = pow_seq(3, n);                     /* 3^n */
    seq_t term2 = seq_mul(5,
                          seq_mul(pow_seq(2, n - 1),
                                  pow_seq(7, n - 2)));  /* 5*2^(n-1)*7^(n-2) */
    seq_t term3 = seq_mul(14,
                          seq_mul(pow_seq(5, n - 3),
                                  pow_seq(31, n - 2)));/* 14*5^(n-3)*31^(n-2) */
    seq_t term4 = seq_mul(10, pow_seq(39, n - 2));     /* 10*39^(n-2) */

    return term1 + term2 + term3 + term4;
}
