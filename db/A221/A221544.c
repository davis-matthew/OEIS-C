/*
 * A221544  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by something other than 1, starting with 0.
 * https://oeis.org/A221544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^5 + 2*n^4 - 6*n^3 + 21*n^2 - 31*n + 23 for n>4 Initial
 *            values: a(1)=5, a(2)=54, a(3)=364, a(4)=1386
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)54;
    if (n == 3) return (seq_t)364;
    if (n == 4) return (seq_t)1386;

    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2  * nn;
    seq_t n4  = n3  * nn;
    seq_t n5  = n4  * nn;

    seq_t res = n5 + (seq_t)2 * n4;
    res -= (seq_t)6 * n3;
    res += (seq_t)21 * n2;
    res -= (seq_t)31 * nn;
    res += (seq_t)23;

    return res;
}
