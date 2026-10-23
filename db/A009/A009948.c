/*
 * A009948  Coordination sequence for alpha-Nd, Position Nd1.
 * https://oeis.org/A009948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (5 - (-1)^n + 2*(-i)^n + 2*i^n + 42*n^2)/4 for n>0,
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t t;
    if ((n & 1) == 0) {
        if ((n & 3) == 0)       /* n % 4 == 0 */
            t = (seq_t)8;
        else                    /* n % 4 == 2 */
            t = (seq_t)0;
    } else {                    /* n is odd */
        t = (seq_t)6;
    }
    seq_t sq = (seq_t)n * (seq_t)n;       /* n^2 */
    seq_t val = (seq_t)42 * sq + t;       /* 42*n^2 + T */
    return val / (seq_t)4;
}
