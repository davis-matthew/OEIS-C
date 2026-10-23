/*
 * A135421  a(n) = 5^n*6^(n^2).
 * https://oeis.org/A135421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n*6^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Exponentiation by squaring – works for any unsigned seq_t type. */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 5^n */
    seq_t pow5 = pow_seq((seq_t)5, n);

    /* 6^(n^2) – compute n^2 in idx_t, safe for n up to ~4.3e9 */
    idx_t n_sq = n * n;
    seq_t pow6 = pow_seq((seq_t)6, n_sq);

    /* Final product */
    return pow5 * pow6;
}
