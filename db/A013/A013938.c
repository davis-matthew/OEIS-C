/*
 * A013938  a(n) = Sum_{k=1..n} floor(n/k^4).
 * https://oeis.org/A013938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} floor(n/k^4)
 * Range:     u8 n <= 239, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = Σ_{k=1}^{n} ⌊ n / k⁴ ⌋ for all n ≥ 1. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; ; ++k)
    {
        idx_t denom = k * k;   /* k²  */
        denom = denom * denom; /* k⁴  */
        if (denom == 0) break; /* safety for overflow, not expected for n ≤ 10000 */
        idx_t term = n / denom;
        if (term == 0) break;  /* floor(n/k⁴) is zero for larger k */
        sum += (seq_t)term;
    }
    return sum;
}
