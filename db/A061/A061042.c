/*
 * A061042  Denominator of 1/16 - 1/n^2.
 * https://oeis.org/A061042
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = 16*n^2 / gcd(16*n^2, n^2-16)
 * Range:     u8 n <= 4, u16 n <= 63, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Compute gcd of two seq_t values. Works for all available seq_t sizes. */
static seq_t seq_gcd(seq_t a, seq_t b)
{
    while (b != (seq_t)0) {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* The offset is 4, but the result is valid for any n >= 4 */
    seq_t nn = (seq_t)n;          /* cast index to seq_t safely */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t num = (seq_t)16 * n2;   /* 16 * n^2 */
    seq_t den = n2 - (seq_t)16;   /* n^2 - 16 */

    seq_t g = seq_gcd(num, den);
    return num / g;
}
