/*
 * A189274  Number of nX3 array permutations with each element not moved or
 *          moved diagonally or antidiagonally by one.
 * https://oeis.org/A189274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1, else 9 * 5^(n-2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The mult function for seq_t values. */
#include "../utils/seq_mul.h"

/* Exponentiation by squaring for base 5 and exponent exp. */
static seq_t pow_seq5(idx_t exp)
{
    seq_t result = 1;   /* 5^0 = 1 */
    seq_t base   = 5;   /* base 5 */
    while (exp > 0) {
        if (exp & 1) {
            result = seq_mul(result, base);
        }
        base = seq_mul(base, base);
        exp >>= 1;
    }
    return result;
}

/* OEIS sequence A189274 */
seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 9, a(n) = 5 * a(n-1) for n > 2 */
    if (n == 1) {
        return 1;
    }
    if (n == 2) {
        return 9;
    }
    seq_t pow5 = pow_seq5(n - 2);  /* 5^(n-2) */
    return seq_mul(pow5, 9);       /* 9 * 5^(n-2) */
}
