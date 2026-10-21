/*
 * A062828  a(n) = gcd(2n, n(n+1)/2).
 * https://oeis.org/A062828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = gcd(2n, n(n+1)/2)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent piecewise definition: */
/*   a(4k+1) = 4k+1,   a(4k+2) = 2k+1,   a(4k+3) = 8k+6,   a(4k+4) = 2k+2 */
seq_t a(idx_t n)
{
    /* handle modulo 4 */
    switch (n & 3) {
        case 1:               /* 4k+1 */
            return (seq_t)n;
        case 3:               /* 4k+3 */
            return (seq_t)n * (seq_t)2;
        /* cases 0 and 2 correspond to 4k+0 and 4k+2 */
        default:              /* 4k and 4k+2 */
            return (seq_t)(n / 2);
    }
}
