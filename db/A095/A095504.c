/*
 * A095504  a(n) = 56 written in base n.
 * https://oeis.org/A095504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer whose digits are the base‑n
 *            representation of 56
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {                 /* unary: 56 ones */
        seq_t r = 0;
        for (idx_t i = 0; i < 56; ++i)
            r = r * 10 + 1;
        return r;
    }
    seq_t val = 56;                /* number to convert */
    seq_t base = (seq_t)n;         /* convert index to seq_t for arithmetic */
    int digits[10];                /* 56 in base 2 uses at most 6 digits */
    int dcnt = 0;

    /* Convert val to base n, storing remainders */
    while (val > 0) {
        digits[dcnt++] = (int)(val % base);
        val /= base;
    }

    /* Build the decimal integer from most significant digit */
    seq_t result = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
