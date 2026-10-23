/*
 * A106113  5th diagonal of triangle in A059317.
 * https://oeis.org/A106113
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 14*n^3 - 97*n^2 + 154*n - 24)/24 for n > 2, a(0) =
 *            a(1) = a(2) = 0
 * Range:     u8 n <= 7, u16 n <= 32, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    long long nn = (long long)n;
    long long n2  = nn * nn;          /* n^2   */
    long long n3  = n2 * nn;          /* n^3   */
    long long n4  = n3 * nn;          /* n^4   */

    long long num = n4 + 14 * n3 - 97 * n2 + 154 * nn - 24;
    long long ans = num / 24;

    return (seq_t)ans;
}
