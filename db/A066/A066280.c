/*
 * A066280  a(n) = 1^n + 2^(n+1) + 3^(n+2).
 * https://oeis.org/A066280
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1^n + 2^(n+1) + 3^(n+2)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 18, u64 n <= 38, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Helper: binary exponentiation for the seq_t type.
   Accepts a non‑negative exponent; for negative exponents the loop is skipped
   and the function returns 1, which is fine for the requested domain. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp > 0) {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1^n is always 1 for integer n. */
    seq_t term1 = 1;

    /* 2^(n+1) */
    seq_t term2 = pow_seq_t((seq_t)2, n + 1);

    /* 3^(n+2) */
    seq_t term3 = pow_seq_t((seq_t)3, n + 2);

    return term1 + term2 + term3;
}
