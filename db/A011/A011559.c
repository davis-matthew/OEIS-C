/*
 * A011559  Stirling numbers of second kind S2(10,n).
 * https://oeis.org/A011559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = Stirling numbers of the second kind S(10, n)
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static seq_t S[11][11] = {0};
    static int initialized = 0;
    if (!initialized) {
        S[0][0] = (seq_t)1;
        for (int i = 1; i <= 10; i++) {
            for (int j = 1; j <= i; j++) {
                S[i][j] = (seq_t)j * S[i-1][j] + S[i-1][j-1];
            }
        }
        initialized = 1;
    }
    if (n == 0 || n > 10) return (seq_t)0;
    return S[10][(int)n];
}
