/*
 * A079908  Solution to the Dancing School Problem with 3 girls and n+3 boys:
 *          f(3,n).
 * https://oeis.org/A079908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = max(1, n^3 + 3*n)
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t x = (seq_t)n;
    return x * x * x + (seq_t)3 * x;
}
