/*
 * A119771  Product of n^2 and n-th tetrahedral number: a(n) =
 *          n^3*(n+1)*(n+2)/6.
 * https://oeis.org/A119771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^3*(n+1)*(n+2)/6
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t n_s = (seq_t)n;
    seq_t f[5] = { n_s, n_s, n_s, n_s + 1, n_s + 2 };

    /* cancel one factor 2 */
    for (int i = 0; i < 5; ++i) {
        if ((f[i] & 1U) == 0) {
            f[i] /= 2U;
            break;
        }
    }

    /* cancel one factor 3 */
    for (int i = 0; i < 5; ++i) {
        if (f[i] % 3U == 0) {
            f[i] /= 3U;
            break;
        }
    }

    seq_t res = 1;
    for (int i = 0; i < 5; ++i) {
        res *= f[i];
    }
    return res;
}
