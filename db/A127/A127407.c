/*
 * A127407  Negative value of coefficient of x^(n-2) in the characteristic
 *          polynomial of a certain n X n integer circulant matrix.
 * https://oeis.org/A127407
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*n*(-7 + 6*n + n*n)/12
 * Range:     u8 n <= 3, u16 n <= 14, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS sequence A127407.
 * For n < 2 the value is defined as 0. */
seq_t a(idx_t n)
{
    if (n < 2u) return 0;
    seq_t w   = (seq_t)n;          /* w = n */
    seq_t w2  = w * w;             /* w2 = n^2 */
    seq_t part = w2 + 6u * w - 7u; /* part = n^2 + 6*n - 7 */
    seq_t num  = w2 * part;        /* num  = n^2 * (n^2 + 6*n - 7) */
    return num / 12u;              /* a(n) = num / 12 */
}
