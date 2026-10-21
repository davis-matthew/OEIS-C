/*
 * A066274  Number of endofunctions of [n] such that 1 is not a fixed point.
 * https://oeis.org/A066274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-1) * (n-1) (equivalent to n^n - n^(n-1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    seq_t b = base;
    idx_t e = exp;
    while (e > 0) {
        if (e & 1) result *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;               /* safety for non‑positive indices */
    if (n == 1) return (seq_t)0;              /* a(1) = 0 */
    seq_t pow = pow_seq_t((seq_t)n, n - 1);    /* n^(n-1) */
    return pow * (seq_t)(n - 1);              /* n^(n-1) * (n-1) = n^n - n^(n-1) */
}
