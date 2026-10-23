/*
 * A155636  a(n) = 9^n + 5^n - 1.
 * https://oeis.org/A155636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9^n + 5^n - 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer exponentiation by squaring (works for any seq_t that supports +-*). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & (idx_t)1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* OEIS A155636: a(n) = 9^n + 5^n - 1 */
seq_t a(idx_t n)
{
    seq_t nine  = (seq_t)9;
    seq_t five  = (seq_t)5;

    seq_t pow9 = pow_seq(nine, n);
    seq_t pow5 = pow_seq(five, n);

    return pow9 + pow5 - (seq_t)1;
}
