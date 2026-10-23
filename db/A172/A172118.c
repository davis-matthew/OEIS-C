/*
 * A172118  a(n) = n*(n+1)*(5*n^2 - n - 3)/2.
 * https://oeis.org/A172118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(5*n^2 - n - 3)/2
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 202, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;               /* a(0) = 0 */
    seq_t tn  = (seq_t)n;        /* use seq_t for all arithmetic */
    seq_t tn1 = tn + 1;
    seq_t tn2 = tn * tn;         /* n^2 */
    seq_t inner = 5 * tn2 - tn - 3; /* 5*n^2 - n - 3 (non‑negative for n>=1) */
    seq_t prod  = tn * tn1 * inner;
    return prod / 2;
}
