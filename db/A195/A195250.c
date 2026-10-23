/*
 * A195250  Number of lower triangles of a 4X4 0..n array with each element
 *          differing from all of its diagonal, vertical, antidiagonal and
 *          horizontal neighbors by two or less.
 * https://oeis.org/A195250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 176471*n - 349213 for n>4
 * Range:     u8 none, u16 n <= 2, u32 n <= 200, u64 n <= 200, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Exact values for the first four terms: a(1)=1024, a(2)=59049, a(3)=190514, a(4)=357847 */

seq_t a(idx_t n)
{
    if (n <= 4) {
        switch (n) {
        case 1: return (seq_t)1024;
        case 2: return (seq_t)59049;
        case 3: return (seq_t)190514;
        case 4: return (seq_t)357847;
        }
    }
    return (seq_t)176471 * (seq_t)n - (seq_t)349213;
}
