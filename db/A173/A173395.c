/*
 * A173395  a(n) = (A002260(n) + 1) * (A004736(n) + 1).
 * https://oeis.org/A173395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (A002260(n)+1) * (A004736(n)+1) * This computes the a(n)
 *            value by determining the diagonal * index d such that d(d+1)/2 ≥
 *            n and the position p on that * diagonal, then returns
 *            (p+1)*(d+2-p)
 * Range:     u8 n <= 420, u16 n <= 9944, u32 n <= 9944, u64 n <= 9944,
 *            u128 n <= 9944, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find smallest d with d(d+1)/2 >= n (diagonal number) */
    idx_t d = 1;
    idx_t t = 1;                 /* t = d(d+1)/2 */
    while (t < n) {
        ++d;
        t = d * (d + 1) / 2;
    }

    /* Previous triangular number T_{d-1} = t - d */
    idx_t p = n - (t - d);        /* position on diagonal (1-based) */

    seq_t i = (seq_t)p;                      /* row index (0‑based) */
    seq_t j = (seq_t)(d + 1 - p);             /* column index (0‑based) */

    /* a(n) = (i+1)*(j+1) */
    return (i + 1) * (j + 1);
}
