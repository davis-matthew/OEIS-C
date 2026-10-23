/*
 * A038586  Write n in ternary then sort the digits.
 * https://oeis.org/A038586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the decimal number obtained from the base‑3 expansion of
 *            n with its digits sorted in non‑decreasing order
 * Range:     u8 n <= 39, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned cnt[3] = {0, 0, 0};

    if (n == 0) {
        cnt[0] = 1;
    } else {
        idx_t m = n;
        while (m > 0) {
            unsigned digit = (unsigned)(m % 3);
            ++cnt[digit];
            m /= 3;
        }
    }

    seq_t result = 0;
    for (unsigned d = 0; d < 3; ++d) {
        for (unsigned i = 0; i < cnt[d]; ++i) {
            result = result * 10 + d;
        }
    }
    return result;
}
