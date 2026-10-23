/*
 * A049486  Maximum length of non-crossing path on n X n square lattice.
 * https://oeis.org/A049486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, nice, easy
 * Formula:   a(n) = (9 + (-1)^n - 8*n + 4*n^2)/2, for n > 2; special cases:
 *            a(1) = 1, a(2) = 4
 * Range:     u8 n <= 9, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;

    seq_t nn = (seq_t)n;                     /* cast index to seq_t */
    seq_t val = nn * nn;                     /* n^2      */
    val *= (seq_t)4;                         /* 4*n^2    */
    seq_t sub = nn * (seq_t)8;               /* 8*n      */
    val -= sub;                              /* 4*n^2-8*n */

    /* (-1)^n: +1 if n even, -1 if n odd */
    int64_t base_signed = 9 + ((n & 1) ? -1 : 1); /* 8 for odd, 10 for even */

    return (val + (seq_t)base_signed) / (seq_t)2;
}
