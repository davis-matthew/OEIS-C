/*
 * A206262  Number of (n+1) X 4 0..1 arrays with the number of rightwards and
 *          downwards edge increases in each 2 X 2 subblock equal to the
 *          number in all its horizontal and vertical neighbors.
 * https://oeis.org/A206262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A206262 – Closed‑form formula
   a(1) = 34;
   for n > 1:
     if n is even:
        a(n) = (336 + 154*n + 71*n^2 + 14*n^3 + n^4) / 24
     if n is odd:
        a(n) = (312 + 154*n + 71*n^2 + 14*n^3 + n^4) / 24
   (Integer arithmetic) */
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)34;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;

    seq_t num;
    if ((nn & (seq_t)1) == 0)            /* even n */
        num = (seq_t)336 + (seq_t)154 * nn + (seq_t)71 * n2 + (seq_t)14 * n3 + n4;
    else                                 /* odd n */
        num = (seq_t)312 + (seq_t)154 * nn + (seq_t)71 * n2 + (seq_t)14 * n3 + n4;

    return num / (seq_t)24;
}
