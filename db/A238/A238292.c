/*
 * A238292  Final digit of imaginary part of (n+n*i)^n.
 * https://oeis.org/A238292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = last digit of Im[(n + n*i)^n]. Im[(n + n*i)^n] = n^n *
 *            2^{⌊n/2⌋} * s, where s = 0 if n%4==0, +1 if n%8∈{1,2,3}, -1 if
 *            n%8∈{5,6,7}. The last digit is ((n^n mod 10) * (2^{⌊n/2⌋} mod
 *            10)) mod 10, negated (i.e., replaced by 10−x mod 10) when s = –1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   pow_mod10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_mod10.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    /* n == 0 is outside the offset, return any value (0). */
    if (n == 0)
        return (seq_t)0;

    /* If n is a multiple of 4, the imaginary part is 0. */
    if ((n & (idx_t)3) == (idx_t)0)
        return (seq_t)0;

    /* Sign factor: +1 for n%8∈{1,2,3}, –1 otherwise. */
    int sign_is_negative = ((n % (idx_t)8) > (idx_t)3);

    /* Compute n^n mod 10. */
    seq_t n_mod10 = (seq_t)(n % (idx_t)10);
    seq_t pow_n = pow_mod10(n_mod10, n);

    /* Compute 2^{⌊n/2⌋} mod 10. */
    idx_t half_exp = n >> 1;          /* floor(n/2) */
    seq_t pow_two = pow_mod10((seq_t)2, half_exp);

    /* Multiply and reduce mod 10. */
    seq_t prod = (seq_t)((pow_n * pow_two) % (seq_t)10);

    /* Adjust sign if necessary. */
    if (sign_is_negative) {
        if (prod == (seq_t)0)
            prod = (seq_t)0;
        else
            prod = (seq_t)((10 - prod) % (seq_t)10);
    }

    return prod;
}
