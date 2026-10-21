/*
 * A070333  Expansion of (1 + x)*(1 - x + x^2)/((1 - x)^4*(1 + x + x^2)).
 * https://oeis.org/A070333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round((2*n + 3)*(n^2 + 3*n + 6)/18)
 * Range:     u8 n <= 3, u16 n <= 30, u32 n <= 1807, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

//          (equivalent to ( (2*n+3)*(n*n+3*n+6) + 9 ) / 18 )
seq_t a(idx_t n)
{
    /* Compute (2*n+3) and (n^2 + 3*n + 6) using seq_t to avoid overflow */
    seq_t term1 = (seq_t)(2 * n + 3);
    seq_t term2 = (seq_t)(n * n + 3 * n + 6);
    seq_t prod   = term1 * term2;  /* (2*n+3)*(n^2 + 3*n + 6) */
    /* Round to nearest integer: add 9 (half of 18) before integer division */
    return (prod + 9) / 18;
}
