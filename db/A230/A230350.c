/*
 * A230350  Number of permutations of [2n+10] in which the longest increasing
 *          run has length n+10.
 * https://oeis.org/A230350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=22, for n≥2 * a(n) = (n^3+22*n^2+132*n+131) *
 *            ∏_{k=n+13}^{2*n+10} k * which equals
 *            (n^3+22*n^2+132*n+131)*(2*n+10)!/(n+12)! for n≥2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)22;

    /* Compute the polynomial factor (n^3+22*n^2+132*n+131) */
    seq_t poly = (seq_t)n * n * n
               + (seq_t)22 * n * n
               + (seq_t)132 * n
               + (seq_t)131;

    /* Compute the product ratio (2n+10)!/(n+12)! = ∏_{k=n+13}^{2*n+10} k */
    seq_t result = poly;
    for (idx_t k = n + 13; k <= 2 * n + 10; ++k)
    {
        result *= (seq_t)k;
    }
    return result;
}
