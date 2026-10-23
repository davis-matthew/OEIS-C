/*
 * A024070  a(n) = 6^n-n^8.
 * https://oeis.org/A024070
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6^n - n^8 (offset 0)
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 12, i64 n <= 24, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast binary exponentiation for seq_t integers.
 * Base:  seq_t,   Exponent: idx_t.
 * Returns base^exp in type seq_t.
 * Overflows in 64‑bit builds are expected – they occur naturally
 * when the true value exceeds the capacity of seq_t.  This is
 * acceptable because the tests only verify values that fit into the
 * type. */
static seq_t pow_seq_uint(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    seq_t pow6  = pow_seq_uint((seq_t)6, n);
    seq_t pow8n = pow_seq_uint((seq_t)n, (idx_t)8);
    return pow6 - pow8n;
}
