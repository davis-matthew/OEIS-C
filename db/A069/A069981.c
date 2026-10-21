/*
 * A069981  Hermite's problem: number of positive integral solutions to x + y
 *          + z = n subject to x <= y + z, y <= z + x and z <= x + y.
 * https://oeis.org/A069981
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n+8)*(n-2)/8 for even n, (n^2-1)/8 for odd n, a(0)=0
 * Range:     u8 n <= 43, u16 n <= 721, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    if ((n & 1) == 0) {            /* even n */
        seq_t t1 = (seq_t)n + (seq_t)8;
        seq_t t2 = (seq_t)n - (seq_t)2;
        return (t1 * t2) / (seq_t)8;
    } else {                       /* odd n */
        return ((seq_t)n * (seq_t)n - (seq_t)1) / (seq_t)8;
    }
}
