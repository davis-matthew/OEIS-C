/*
 * A061165  Polynomial extrapolation of 2, 3, 5, 7, 11.
 * https://oeis.org/A061165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^4 - 34*n^3 + 141*n^2 - 206*n + 144)/24
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 222, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 2, a(2) = 3, a(3) = 5, a(4) = 7, a(5) = 11, ...  (offset 1) */

seq_t a(idx_t n)
{
    /* For n == 0 the sequence is undefined; return 0 as a guard. */
    if (n == 0)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t res = (seq_t)3 * n4;
    res -= (seq_t)34 * n3;
    res += (seq_t)141 * n2;
    res -= (seq_t)206 * nn;
    res += (seq_t)144;
    res /= (seq_t)24;

    return res;
}
