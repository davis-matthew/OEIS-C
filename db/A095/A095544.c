/*
 * A095544  a(n) = 76 written in base n.
 * https://oeis.org/A095544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer whose decimal digits are the base‑n
 *            representation of the decimal number 76. For n==1 the result is
 *            76 ones (unary representation)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the special base‑1 case: 76 ones. */
    if (n == 1) {
        seq_t res = 0;
        for (unsigned i = 0; i < 76; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Compute 76 in base n, collecting the digits. */
    seq_t value = (seq_t)76;
    unsigned int digits[10];       /* length <= 7 for any base >= 2 */
    unsigned int dcnt = 0;
    seq_t base = (seq_t)n;

    while (value > 0) {
        seq_t rem = value % base;
        digits[dcnt++] = (unsigned int)rem;
        value /= base;
    }

    /* Assemble the decimal number from the collected digits. */
    seq_t res = 0;
    for (int i = (int)dcnt - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
