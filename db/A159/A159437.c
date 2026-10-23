/*
 * A159437  Number of n X n arrays of squares of integers summing to 36.
 * https://oeis.org/A159437
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of z^36 in (1 + z + z⁴ + z⁹ + z¹⁶ + z²⁵ +
 *            z³⁶)^{n²}
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t binom(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        res = res * (seq_t)(n - k + i) / i;
    }
    return res;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    idx_t m = n * n;                /* number of entries in the array */
    seq_t total = 0;

    /* Enumerate all solutions to:
       36*c36 + 25*c25 + 16*c16 + 9*c9 + 4*c4 + 1*c1 = 36   (ci ≥ 0)   */
    for (idx_t c36 = 0; c36 <= 1; ++c36) {
        idx_t rem0 = 36 - 36 * c36;
        for (idx_t c25 = 0; c25 <= rem0 / 25; ++c25) {
            idx_t rem1 = rem0 - 25 * c25;
            for (idx_t c16 = 0; c16 <= rem1 / 16; ++c16) {
                idx_t rem2 = rem1 - 16 * c16;
                for (idx_t c9 = 0; c9 <= rem2 / 9; ++c9) {
                    idx_t rem3 = rem2 - 9 * c9;
                    for (idx_t c4 = 0; c4 <= rem3 / 4; ++c4) {
                        idx_t c1 = rem3 - 4 * c4;
                        idx_t sumCnt = c36 + c25 + c16 + c9 + c4 + c1;
                        if (sumCnt > m) continue;

                        seq_t ways = 1;
                        idx_t remaining = m;

                        ways = ways * binom(remaining, c36);
                        remaining -= c36;
                        ways = ways * binom(remaining, c25);
                        remaining -= c25;
                        ways = ways * binom(remaining, c16);
                        remaining -= c16;
                        ways = ways * binom(remaining, c9);
                        remaining -= c9;
                        ways = ways * binom(remaining, c4);
                        remaining -= c4;
                        ways = ways * binom(remaining, c1);
                        remaining -= c1;

                        total += ways;
                    }
                }
            }
        }
    }
    return total;
}
