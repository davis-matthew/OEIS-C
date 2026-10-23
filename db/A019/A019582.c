/*
 * A019582  a(n) = n*(n - 1)^3/2.
 * https://oeis.org/A019582
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n-1)^3/2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 301, u64 n <= 680,
 *            u128 n <= 680, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;
    seq_t nn = (seq_t)n;
    seq_t nm1 = (seq_t)(n - 1);
    seq_t result = nn * nm1;
    result *= nm1;
    result *= nm1;
    result /= 2;
    return result;
}
