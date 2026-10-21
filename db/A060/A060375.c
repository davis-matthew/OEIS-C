/*
 * A060375  a(n) = (n+2)^(n+3) - n^(n+1).
 * https://oeis.org/A060375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+2)^(n+3) - n^(n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Efficient binary exponentiation for arbitrary integer type seq_t.
   Works with any seq_t that supports +, -, * operations and
   with idx_t as the exponent type. */
static inline seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & 1)
            result = result * base;
        exp >>= 1;
        if (exp)
            base = base * base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute (n+2)^(n+3) and n^(n+1) */
    seq_t term1 = pow_seq_t((seq_t)(n + 2), n + 3);
    seq_t term2 = pow_seq_t((seq_t)n, n + 1);
    return term1 - term2;
}
