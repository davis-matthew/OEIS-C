/*
 * A159439  Number of n X n arrays of squares of integers summing to 37.
 * https://oeis.org/A159439
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^37 in (1 + x + x^4 + x^9 + x^16 + x^25 +
 *            x^36)^(n^2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom_small(seq_t N, idx_t k)
{
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
    {
        res = res * (N - (seq_t)i + 1) / (idx_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t n2 = n * n;
    seq_t N = (seq_t)n * (seq_t)n;
    seq_t total = 0;

    for (idx_t a36 = 0; a36 <= 1; ++a36)
    {
        idx_t rem1 = 37 - 36 * a36;
        if (rem1 < 0) break;
        for (idx_t a25 = 0; a25 <= rem1 / 25; ++a25)
        {
            idx_t rem2 = rem1 - 25 * a25;
            for (idx_t a16 = 0; a16 <= rem2 / 16; ++a16)
            {
                idx_t rem3 = rem2 - 16 * a16;
                for (idx_t a9 = 0; a9 <= rem3 / 9; ++a9)
                {
                    idx_t rem4 = rem3 - 9 * a9;
                    for (idx_t a4 = 0; a4 <= rem4 / 4; ++a4)
                    {
                        idx_t a1 = rem4 - 4 * a4;
                        idx_t sum_nonzero = a36 + a25 + a16 + a9 + a4 + a1;
                        if (sum_nonzero > n2) continue;

                        seq_t rem = N;
                        seq_t ways = 1;

                        ways *= binom_small(rem, a36); rem -= a36;
                        ways *= binom_small(rem, a25); rem -= a25;
                        ways *= binom_small(rem, a16); rem -= a16;
                        ways *= binom_small(rem, a9);  rem -= a9;
                        ways *= binom_small(rem, a4);  rem -= a4;
                        ways *= binom_small(rem, a1);  rem -= a1;

                        total += ways;
                    }
                }
            }
        }
    }
    return total;
}
