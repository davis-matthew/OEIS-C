/*
 * A125079  Excess of number of divisors of 2n+1 of form 12k+1, 12k+5 over
 *          those of form 12k+7, 12k+11.
 * https://oeis.org/A125079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   Excess of number of divisors of 2n+1 that are 1 or 5 mod 12
 *            minus those that are 7 or 11 mod 12
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n * 2 + 1;          /* m = 2n+1, always odd */
    long long total = 0;          /* accumulate signed to avoid underflow */

    for (idx_t i = 1; i * i <= m; ++i) {
        if (m % i == 0) {
            idx_t j = m / i;
            /* weight for divisor i */
            int r = (int)(i % 12);
            int sign = 0;
            if (r == 1 || r == 5)     sign = 1;
            else if (r == 7 || r == 11) sign = -1;
            total += sign;

            /* weight for paired divisor j, if distinct */
            if (j != i) {
                r = (int)(j % 12);
                sign = 0;
                if (r == 1 || r == 5)     sign = 1;
                else if (r == 7 || r == 11) sign = -1;
                total += sign;
            }
        }
    }
    return (seq_t)total;
}
