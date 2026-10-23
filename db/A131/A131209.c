/*
 * A131209  Maximal distance between two signed permutations of n elements.
 * https://oeis.org/A131209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1 for all n except a(0)=0, a(1)=1, a(3)=3
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 3) return 3;
    return (seq_t)n + 1;
}
