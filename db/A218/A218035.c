/*
 * A218035  Number of n-digit palindromes with squares that are also
 *          palindromes.
 * https://oeis.org/A218035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = (n³-6*n²+32*n+48)/48 if n even; (n³-9*n²+59*n-3)/24 if n
 *            odd>1; 4 if n = 1
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 2706, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return 4;

    seq_t N  = (seq_t)n;
    seq_t N2 = N * N;
    seq_t N3 = N2 * N;
    seq_t num;

    if ((n & 1) == 0) {          /* n even */
        num = N3 - (seq_t)6 * N2 + (seq_t)32 * N + (seq_t)48;
        return num / (seq_t)48;
    } else {                     /* n odd, n > 1 */
        num = N3 - (seq_t)9 * N2 + (seq_t)59 * N - (seq_t)3;
        return num / (seq_t)24;
    }
}
