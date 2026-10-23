/*
 * A028309  Molien series for ring of symmetrized weight enumerators of
 *          self-dual codes (with respect to Euclidean inner product) of
 *          length n over GF(4).
 * https://oeis.org/A028309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8)*(2*n*n - 4*n + 21 + 3*(-1)^n) for n >= 3; a(0)=1,
 *            a(1)=1, a(2)=2
 * Range:     u8 n <= 11, u16 n <= 184, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    seq_t nn = (seq_t)n;
    seq_t num = (seq_t)2 * nn * nn;   /* 2*n^2 */
    num -= (seq_t)4 * nn;             /* - 4*n */
    num += (seq_t)21;                 /* + 21 */

    /* add or subtract 3 depending on parity of n */
    if ((n & 1) == 0) {
        num += (seq_t)3;              /* (-1)^n = +1 */
    } else {
        num -= (seq_t)3;              /* (-1)^n = -1 */
    }

    return num / (seq_t)8;            /* divide by 8 */
}
