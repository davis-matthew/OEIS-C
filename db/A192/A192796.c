/*
 * A192796  Molecular topological indices of the crown graphs.
 * https://oeis.org/A192796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n-1)*n*(4*n-1)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 813, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for n = 0 the formula would underflow; return 0 safely */
    if (n <= 1) return 0;     /* a(1) = 0 */

    seq_t N = (seq_t)n;
    seq_t result = 2 * (N - 1);
    result *= N;
    result *= (4 * N - 1);
    return result;
}
