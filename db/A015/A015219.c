/*
 * A015219  Odd tetrahedral numbers: a(n) = (4*n+1)*(4*n+2)*(4*n+3)/6.
 * https://oeis.org/A015219
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n+1)*(4*n+2)*(4*n+3)/6
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 736, u64 n <= 9915,
 *            u128 n <= 9915, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient C(4*n+3, 3) in an overflow‑safe way. */
    seq_t res = 1;
    seq_t m = (seq_t)(4*n + 3);
    for (int k = 1; k <= 3; ++k)
    {
        res = res * (m - k + 1) / (seq_t)k;
    }
    return res;
}
