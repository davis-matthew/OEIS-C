/*
 * A183655  Number of (n+1) X 3 0..5 arrays with every 2 X 2 subblock summing
 *          to 10.
 * https://oeis.org/A183655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*6^n + 30*5^n + 48*4^n + 54*3^n + 48*2^n + 30 (OEIS
 *            A183655)
 * Range:     u8 none, u16 n <= 4, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: fast integer power for seq_t base, idx_t exponent.
   Uses simple repeated multiplication; O(exponent) but fast for n <= 10000. */
static inline seq_t pow_seq_local(seq_t base, idx_t exp)
{
    seq_t result = 1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    /* For the offset 1, n >= 1. We compute the closed‑form expression. */
    seq_t t6 = pow_seq_local(6, n);
    seq_t t5 = pow_seq_local(5, n);
    seq_t t4 = pow_seq_local(4, n);
    seq_t t3 = pow_seq_local(3, n);
    seq_t t2 = pow_seq_local(2, n);

    seq_t result = 6 * t6 + 30 * t5 + 48 * t4 + 54 * t3 + 48 * t2 + 30;
    return result;
}
