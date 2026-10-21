/*
 * A072259  a(n) = ((6*n+37)*4^n - 1)/3.
 * https://oeis.org/A072259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((6*n+37)*4^n - 1)/3
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 28, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS A072259, with offset 0. */
#include <stdint.h> /* for uint64_t, if needed by the build */

seq_t a(idx_t n)
{
    /* Binary exponentiation to compute 4^n efficiently. */
    seq_t pow4 = 1;
    seq_t base = 4;
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            pow4 *= base;
        }
        base *= base;
        exp >>= 1;
    }

    /* numerator = (6*n + 37) * 4^n - 1 */
    seq_t six_n = (seq_t)6 * (seq_t)n;
    seq_t num_term = six_n + (seq_t)37;
    seq_t numerator = num_term * pow4 - (seq_t)1;

    /* divide by 3 (exact division). */
    return numerator / (seq_t)3;
}
