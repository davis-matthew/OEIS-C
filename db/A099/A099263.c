/*
 * A099263  a(n) = (1/40320)*8^n + (1/1440)*6^n + (1/360)*5^n + (1/64)*4^n +
 *          (11/180)*3^n + (53/288)*2^n + 103/280. Partial sum of Stirling
 *          numbers of second kind S(n,i), i=1..8 (i.e., a(n) = Sum_{i=1..8}
 *          S(n,i)).
 * https://oeis.org/A099263
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..8} S(n,k) (sum of Stirling numbers of the
 *            second kind up to k=8)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; undefined for n < 1 */
    seq_t S[9] = {0};
    S[0] = 1;                      /* S(0,0) = 1 */

    for (idx_t i = 1; i <= n; ++i) {
        for (idx_t k = 8; k >= 1; --k) {
            S[k] = (seq_t)k * S[k] + S[k - 1];
        }
        S[0] = 0;                  /* S(n,0) = 0 for n > 0 */
    }

    seq_t sum = 0;
    for (idx_t k = 1; k <= 8; ++k) {
        sum += S[k];
    }
    return sum;
}
