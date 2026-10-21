/*
 * A095444  a(n) = 26 written in base n.
 * https://oeis.org/A095444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 26 written in base n
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 13, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base‑1 (unary) is a special case: 26 ones. */
    if (n == 1) {
        seq_t result = 0;
        for (idx_t i = 0; i < 26; ++i) {
            result = result * 10 + 1;
        }
        return result;
    }

    /* Convert 26 to base n, collecting digits in reverse order. */
    idx_t val = 26;
    seq_t digits[32];
    int dpos = 0;
    while (val > 0) {
        digits[dpos++] = val % n;
        val /= n;
    }

    /* Build the concatenated decimal representation. */
    seq_t result = 0;
    for (int i = dpos - 1; i >= 0; --i) {
        seq_t d = digits[i];
        /* Digit d is at most 26, so 1 or 2 decimal digits. */
        seq_t mul10 = (d >= 10) ? 100 : 10;
        result = result * mul10 + d;
    }

    return result;
}
