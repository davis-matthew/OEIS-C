/*
 * A024198  4th elementary symmetric function of the first n+3 odd positive
 *          integers.
 * https://oeis.org/A024198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            n*(n+1)*(n+2)*(n+3)*(15*n^4+150*n^3+515*n^2+672*n+223)/360
 * Range:     u8 none, u16 n <= 4, u32 n <= 9, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* zero or negative indices are not defined for this sequence;
       we return 0 for n==0 to keep the function total. */
    if (n == 0)
        return 0;

    /* convert index to seq_t to avoid intermediate overflow of the
       64‑bit arithmetic in the computation. */
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;          /* x^2 */
    seq_t x3 = x2 * x;         /* x^3 */
    seq_t x4 = x2 * x2;        /* x^4 */

    /* Compute the polynomial part */
    seq_t poly = (seq_t)15 * x4
              + (seq_t)150 * x3
              + (seq_t)515 * x2
              + (seq_t)672 * x
              + (seq_t)223;

    /* Compute the (n)(n+1)(n+2)(n+3) part */
    seq_t f1   = x * (x + 1);
    seq_t f2   = (x + 2) * (x + 3);
    seq_t prod = f1 * f2; /* product of four consecutive integers */

    /* Final value: divide by 360 (exact division). */
    return (prod * poly) / (seq_t)360;
}
