/*
 * A095570  a(n) = 89 written in base n.
 * https://oeis.org/A095570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal value obtained by writing 89 in base n and
 *            interpreting the digit string as a decimal integer. For n = 1
 *            the representation is simply 89 copies of the digit '1'
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: base 1 (unary) – 89 copies of 1 */
    if (n == 1) {
        seq_t r = 0;
        for (idx_t i = 0; i < 89; ++i)
            r = r * 10 + 1;
        return r;
    }

    /* 89 expressed in base n: store digits least‑to‑most */
    const seq_t TARGET = 89;
    seq_t value = TARGET;
    /* Enough to hold all digits – 89 has at most 7 bits */
    int digits[20];
    idx_t count = 0;

    while (value > 0) {
        seq_t rem = value % (seq_t)n;
        digits[count++] = (int)rem;
        value = value / (seq_t)n;
    }

    /* Build the decimal number from most significant to least */
    seq_t result = 0;
    for (idx_t i = count; i > 0; --i) {
        seq_t digit = (seq_t)digits[i - 1];

        /* Determine how many decimal digits the current digit has */
        int dlen = 0;
        seq_t tmp = digit;
        if (tmp == 0)
            dlen = 1;
        else {
            while (tmp > 0) {
                tmp /= 10;
                ++dlen;
            }
        }

        /* Multiply result by 10^dlen */
        seq_t pow10 = 1;
        for (int k = 0; k < dlen; ++k)
            pow10 *= 10;

        result = result * pow10 + digit;
    }

    return result;
}
