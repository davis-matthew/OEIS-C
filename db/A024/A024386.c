/*
 * A024386  [ (3rd elementary symmetric function of S(n))/(first elementary
 *          symmetric function of S(n)) ], where S(n) = {first n+2 positive
 *          integers congruent to 1 mod 4}.
 * https://oeis.org/A024386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(4n^2+8n-3)/6
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 249, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* n*(n+1)/2 is always integer */
    seq_t A = nn * (nn + 1) / 2;
    /* 4n^2 + 8n - 3 */
    seq_t term = 4 * nn * nn + 8 * nn - (seq_t)3;
    /* multiply then divide by 3 */
    seq_t prod = A * term;
    return prod / (seq_t)3;
}
