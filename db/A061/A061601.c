/*
 * A061601  9's complement of n: a(n) = 10^d - 1 - n where d is the number of
 *          digits in n. If a is a digit in n replace it with 9 - a.
 * https://oeis.org/A061601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^d − 1 − n, where d = number of decimal digits of n
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 9642, u64 n <= 9642,
 *            u128 n <= 9642, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Precomputed powers of 10 up to 10^20 as seq_t. */
    static seq_t pow10[21] = {0};
    if (!pow10[1]) {
        pow10[1] = (seq_t)10;
        for (int i = 2; i <= 20; ++i)
            pow10[i] = pow10[i - 1] * (seq_t)10;
    }

    /* Determine the number of decimal digits d. */
    idx_t tmp = n;
    uint8_t d = 1;               /* default for n == 0   */
    if (tmp > 0) {
        d = 0;
        while (tmp) {
            ++d;
            tmp /= 10;
        }
    }

    /* Compute 10^d − 1 − n. */
    seq_t res = pow10[d] - (seq_t)1 - (seq_t)n;
    return res;
}
