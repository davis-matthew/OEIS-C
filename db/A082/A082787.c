/*
 * A082787  a(n) = (2/3)*(2*n-1)!*binomial(3*n,2*n).
 * https://oeis.org/A082787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ∏_{k=n+1}^{3n-1} k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t start = n + 1;
    idx_t end   = 3 * n;           /* upper bound is exclusive (3n‑1) */
    for (idx_t k = start; k < end; ++k) {
        result *= (seq_t)k;
    }
    return result;
}
