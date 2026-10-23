/*
 * A042862  Numerators of continued fraction convergents to sqrt(963).
 * https://oeis.org/A042862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 31, a(1) = 962, a(2) = 59675, a(3) = 1850887, a(n) =
 *            1924*a(n-2) - a(n-4) for n >= 4
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes a(n) of OEIS sequence A042862 */
seq_t a(idx_t n)
{
    /* buffer for the last five terms; indices modulo 5 */
    seq_t buf[5];
    buf[0] = (seq_t)31;
    buf[1] = (seq_t)962;
    buf[2] = (seq_t)59675;
    buf[3] = (seq_t)1850887;

    if (n < 4)
        return buf[(size_t)n];

    for (idx_t i = 4; i <= n; ++i)
    {
        size_t idx  = (size_t)(i % 5);
        size_t im2  = (size_t)((i - 2) % 5);
        size_t im4  = (size_t)((i - 4) % 5);
        buf[idx] = (seq_t)(1924 * buf[im2] - buf[im4]);
    }

    return buf[(size_t)(n % 5)];
}
