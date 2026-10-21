/*
 * A089357  a(n) = 2^(6*n).
 * https://oeis.org/A089357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 64^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth term of OEIS sequence A089357: a(0)=1, a(n)=64*a(n-1) */

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;                     /* a(0) = 1 */
    for (idx_t i = 0; i < n; ++i) {           /* a(n) = 64^n */
        res *= (seq_t)64;                     /* multiply by 64 each step */
    }
    return res;
}
