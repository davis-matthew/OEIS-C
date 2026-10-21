/*
 * A074877  Number of function calls required to compute ack(3,n), where ack
 *          denotes the Ackermann function.
 * https://oeis.org/A074877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (128*4^n - 120*2^n + 9*n + 37) / 3 (n ≥ 0)
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation: res = base^exp */
static seq_t pow_uint(seq_t base, idx_t exp)
{
    seq_t res = 1;
    while (exp)
    {
        if (exp & 1)
            res *= base;          /* res *= base */
        base *= base;              /* base *= base */
        exp >>= 1;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* compute usin gpow 4^n and 2^n */
    seq_t pow4 = pow_uint((seq_t)4, n);
    seq_t pow2 = pow_uint((seq_t)2, n);

    /* 128*4^n  */
    seq_t term_mul4 = (seq_t)128 * pow4;
    /* -120*2^n */
    seq_t term_mul2 = (seq_t)120 * pow2;
    /* 9*n */
    seq_t term_muln = (seq_t)9 * (seq_t)n;
    /* +37 */
    seq_t term_const = (seq_t)37;

    /* numerator: 128*4^n - 120*2^n + 9*n + 37 */
    seq_t numerator = term_mul4 - term_mul2 + term_muln + term_const;

    /* divide by 3 (exact) */
    return numerator / (seq_t)3;
}
