/*
 * A226233  Ten copies of each positive integer.
 * https://oeis.org/A226233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)/10) + 1 (n ≥ 1) – each integer is repeated 10
 *            times
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n-th term of A226233, indexed from 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n < 1 */
    idx_t n1 = n - 1;                     /* (n-1) in idx_t to avoid underflow */
    seq_t v = (seq_t)n1;                  /* cast to result type */
    v = v / 10;                           /* integer division by 10 */
    v = v + 1;                            /* add 1 to shift to 1‑based values */
    return v;
}
