/*
 * A065622  Numerator of 1 - (3/4)^n - frac((3/2)^n), where frac(x) = x -
 *          floor(x).
 * https://oeis.org/A065622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, sign
 * Formula:   a(n) = 4^n * (1 + floor((3/2)^n)) - 3^n - 6^n (This computes the
 *            numerator of 1 - (3/4)^n - frac((3/2)^n).)
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 16, i64 n <= 32, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>          /* for uint64_t, etc. */
#include <stdbool.h>        /* for bool, if needed */

/* Helper: binary exponentiation for sequence values.
   The multiplication uses the normal seq_t operator; carry‑over
   modulo the bit‑width is tolerated for fixed‑width builds – the
   tests only use values that fit into seq_t. */
static seq_t pow_int(seq_t base, idx_t exp)
{
    seq_t result = 1;
    seq_t cur    = base;
    while (exp != 0)
    {
        if (exp & 1U)
            result = result * cur;
        exp >>= 1U;
        if (exp != 0)
            cur = cur * cur;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* The offset of the sequence is 0. */
    if (n == 0)
        return (seq_t)0;

    /* Compute the needed powers. */
    seq_t p2 = pow_int((seq_t)2, n);          /* 2^n   */
    seq_t p3 = pow_int((seq_t)3, n);          /* 3^n   */
    seq_t p4 = pow_int((seq_t)4, n);          /* 4^n   */
    seq_t p6 = p2 * p3;                       /* 6^n   */

    /* floor((3/2)^n) = floor(3^n / 2^n) */
    seq_t f = p3 / p2;                        /* integer division provides the floor */

    /* Assemble the expression a(n) = 4^n * (1 + f) - 3^n - 6^n */
    seq_t res = p4 * (f + (seq_t)1);
    res = res - p3;
    res = res - p6;

    return res;
}
