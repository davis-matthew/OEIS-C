/*
 * A074512  a(n) = 1^n + 4^n + 6^n.
 * https://oeis.org/A074512
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1^n + 4^n + 6^n
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Fast exponentiation by squaring, works for any seq_t (built‑in or big integer). */
static seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            result *= base;
        }
        base *= base;
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* 1^n is always 1, so we only need 4^n and 6^n */
    seq_t pow4 = pow_seq((seq_t)4, n);
    seq_t pow6 = pow_seq((seq_t)6, n);
    return (seq_t)1 + pow4 + pow6;
}
