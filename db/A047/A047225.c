/*
 * A047225  Numbers that are congruent to {0, 1} mod 6.
 * https://oeis.org/A047225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n - 3 if n is odd * a(n) = 3*n - 5 if n is even * (for
 *            n >= 1, offset 1: a(1)=0, a(2)=1, a(3)=6, a(4)=7, …)
 * Range:     u8 n <= 86, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle any index below the offset; undefined behaviour otherwise */
    if (n == 0) return (seq_t)0;

    seq_t thr = (seq_t)3 * (seq_t)n;

    if ((n & 1) == 0) {            /* even n */
        return thr - (seq_t)5;
    } else {                       /* odd n */
        return thr - (seq_t)3;
    }
}
