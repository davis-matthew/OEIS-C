/*
 * A057169  Least integer with the same nonzero decimal digits as n and one
 *          more 0 digit.
 * https://oeis.org/A057169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base, look
 * Formula:   a(n) = smallest integer that contains the same non‑zero decimal
 *            digits as n plus one additional 0 digit
 * Range:     u8 n <= 29, u16 n <= 6999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                 /* defined only for n >= 1 */

    /* Count each decimal digit of n. */
    int cnt[10] = {0};
    idx_t tmp = n;
    while (tmp > 0) {
        int d = tmp % 10;
        tmp /= 10;
        cnt[d]++;
    }

    /* Identify the smallest non‑zero digit to use as the leading digit. */
    int min_nonzero = -1;
    for (int d = 1; d <= 9; ++d) {
        if (cnt[d] > 0) {
            min_nonzero = d;
            break;
        }
    }

    /* Add one extra zero and consume the leading non‑zero digit. */
    ++cnt[0];
    --cnt[min_nonzero];

    /* Build the result: leading digit followed by all remaining digits
       in ascending order. */
    seq_t result = (seq_t)min_nonzero;
    for (int d = 0; d <= 9; ++d) {
        for (int k = 0; k < cnt[d]; ++k) {
            result = result * 10 + (seq_t)d;
        }
    }

    return result;
}
