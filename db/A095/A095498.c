/*
 * A095498  a(n) = 53 written in base n.
 * https://oeis.org/A095498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by concatenating the base‑n
 *            digits of 53
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: base 1 → 53 ones */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < 53; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Decompose 53 in base n */
    uint64_t value = 53;
    int digits[64];     /* enough for all possible bases (max 6 digits) */
    int dlen = 0;

    while (value > 0) {
        uint64_t r = value % n;    /* digit in base n */
        digits[dlen++] = (int)r;
        value /= n;
    }

    /* Concatenate the decimal representations of the digits */
    seq_t val = 0;
    for (int i = dlen - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t mult = (d >= 10) ? 100 : 10; /* 10^(number of decimal digits of d) */
        val = val * mult + d;
    }
    return val;
}
