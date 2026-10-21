/*
 * A065331  Largest 3-smooth divisor of n.
 * https://oeis.org/A065331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = the largest divisor of n whose prime factors are only 2
 *            and 3 * (largest 3‑smooth divisor). It equals 2^v2(n) * 3^v3(n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;            /* result: product of powers of 2 and 3 */
    idx_t t   = n;            /* copy of n to factor out 2s and 3s */

    /* remove all factors of 2 */
    while ((t & (idx_t)1) == 0) {
        res *= (seq_t)2;
        t >>= 1;                 /* divide by 2 */
    }

    /* remove all factors of 3 */
    while (t % 3 == 0) {
        res *= (seq_t)3;
        t /= 3;
    }

    return res;                /* res <= n, fits in seq_t whenever n does */
}
