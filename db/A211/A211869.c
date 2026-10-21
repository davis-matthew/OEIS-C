/*
 * A211869  a(n) = Sum_{j=1..n-1} j*(n-j)*b^(j-1) with b = floor(n^2/4)+1.
 * https://oeis.org/A211869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = Sum_{j=1..n-1} j*(n-j)*b^(n-1-j) with b = floor(n^2/4)+1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1u) return (seq_t)0;
    seq_t val  = (seq_t)0;
    seq_t nn   = (seq_t)n;
    seq_t base = nn * nn / (seq_t)4 + (seq_t)1;   /* floor(n^2/4)+1 */

    for (idx_t j = 1u; j < n; ++j) {
        seq_t d = (seq_t)j * (seq_t)(n - j);      /* j*(n-j) */
        val = val * base + d;
    }
    return val;
}
