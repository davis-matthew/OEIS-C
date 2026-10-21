/*
 * A232984  The Gauss factorial n_10!.
 * https://oeis.org/A232984
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = \prod_{1\le j\le n,\;gcd(j,10)=1} j
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 26, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t j = 1; j <= n; ++j) {
        if ((j % 2) != 0 && (j % 5) != 0) {
            res *= (seq_t)j;
        }
    }
    return res;
}
