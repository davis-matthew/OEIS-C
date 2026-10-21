/*
 * A052472  Number of independent components for a Weyl tensor in n
 *          dimensions.
 * https://oeis.org/A052472
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(n+2)*(n-3)/12
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 475, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);
    prod *= (nn + 2);
    prod *= (nn - 3);
    return prod / 12;
}
