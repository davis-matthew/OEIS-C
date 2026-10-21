/*
 * A061040  Denominator of 1/9 - 1/n^2.
 * https://oeis.org/A061040
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, frac, nice, easy
 * Formula:   a(n) = (9*n^2) / gcd(9*n^2 , n^2-9) (denominator of 1/9 - 1/n^2)
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t gcd_seq_t(seq_t a, seq_t b)
{
    while (b != 0)
    {
        seq_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n)
{
    if (n < 3)   /* values below the offset are irrelevant */
        return 0;

    seq_t sq  = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t num = sq - (seq_t)9;                /* n^2 – 9 */
    seq_t den = (seq_t)9 * sq;                /* 9*n^2 */

    seq_t g = gcd_seq_t(den, num);            /* gcd(9*n^2 , n^2-9) */
    return den / g;                          /* reduced denominator */
}
