/*
 * A217397  Numbers starting with 4.
 * https://oeis.org/A217397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = n + (35*10^floor(log10(9*n-8))-8)/9
 * Range:     u8 n <= 11, u16 n <= 1111, u32 n <= 1111, u64 n <= 1111,
 *            u128 n <= 1111, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 9*n - 8, all within idx_t for expected n */
    idx_t m = 9*n - 8;

    /* digits of m: (digits-1) = floor(log10(m)) */
    idx_t digits = 0;
    idx_t t = m;
    do {
        t /= 10;
        ++digits;
    } while (t);

    idx_t k = digits - 1;

    /* compute 10^k in seq_t */
    seq_t pow10 = 1;
    for (idx_t i = 0; i < k; ++i)
        pow10 *= 10;

    /* (35*10^k - 8)/9 */
    seq_t add = ((seq_t)35 * pow10 - 8) / 9;

    return (seq_t)n + add;
}
