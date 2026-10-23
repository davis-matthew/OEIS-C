/*
 * A139400  Number of spanning trees in the graph P_6 x P_n.
 * https://oeis.org/A139400
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = A001906(n) * A001353(n) * A004254(n) * A161498(n) The
 *            four constituent sequences satisfy linear recurrences: f3[n] =
 *            3*f3[n-1] - f3[n-2] (root 3) f4[n] = 4*f4[n-1] - f4[n-2] (root
 *            4) f5[n] = 5*f5[n-1] - f5[n-2] (root 5) ar[n], br[n] satisfy
 *            ar[n] = 4*ar[n-1] + 3*br[n-1] - ar[n-2] br[n] = 4...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                    /* undefined for offset 1 */

    /* Hold the two most recent terms for each sequence. */
    seq_t f3_prev1 = 1, f3_prev2 = 0;   /* f3[i-1], f3[i-2] */
    seq_t f4_prev1 = 1, f4_prev2 = 0;
    seq_t f5_prev1 = 1, f5_prev2 = 0;
    seq_t ar_prev1 = 1, ar_prev2 = 0;
    seq_t br_prev1 = 0, br_prev2 = 0;

    if (n >= 2) {
        f3_prev2 = 3;      /* f3[2] */
        f4_prev2 = 4;      /* f4[2] */
        f5_prev2 = 5;      /* f5[2] */
        ar_prev2 = 4;      /* ar[2] */
        br_prev2 = 1;      /* br[2] */
    }

    for (idx_t i = 3; i <= n; ++i) {
        seq_t f3_cur = 3 * f3_prev2 - f3_prev1;
        seq_t f4_cur = 4 * f4_prev2 - f4_prev1;
        seq_t f5_cur = 5 * f5_prev2 - f5_prev1;
        seq_t ar_cur = 4 * ar_prev2 + 3 * br_prev2 - ar_prev1;
        seq_t br_cur = 4 * br_prev2 + ar_prev2 - br_prev1;

        f3_prev1 = f3_prev2; f3_prev2 = f3_cur;
        f4_prev1 = f4_prev2; f4_prev2 = f4_cur;
        f5_prev1 = f5_prev2; f5_prev2 = f5_cur;
        ar_prev1 = ar_prev2; ar_prev2 = ar_cur;
        br_prev1 = br_prev2; br_prev2 = br_cur;
    }

    seq_t f3n = (n == 1 ? f3_prev1 : f3_prev2);
    seq_t f4n = (n == 1 ? f4_prev1 : f4_prev2);
    seq_t f5n = (n == 1 ? f5_prev1 : f5_prev2);
    seq_t arn = (n == 1 ? ar_prev1 : ar_prev2);
    seq_t brn = (n == 1 ? br_prev1 : br_prev2);

    seq_t prod = f3n;
    prod = prod * f4n;
    prod = prod * f5n;

    seq_t w = arn * arn;
    w = w - (seq_t)3 * brn * brn;

    prod = prod * w;

    return prod;
}
