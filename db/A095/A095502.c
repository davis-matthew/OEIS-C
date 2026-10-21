/*
 * A095502  a(n) = 55 written in base n.
 * https://oeis.org/A095502
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal value of the representation of 55 in base n For n
 *            = 1, a(1) is the 55‑digit number consisting of all 1’s For n >=
 *            2, a(n) is the integer obtained by writing 55 in base n and
 *            treating the base‑n digit string as a decimal number
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 55 ones: iterate 55 times multiplying by 10 and adding 1 */
        seq_t x = 0;
        for (int i = 0; i < 55; ++i)
            x = x * 10 + 1;
        return x;
    }

    /* Convert 55 to base n (value fits in seq_t). */
    seq_t val = 55;
    /* At most 6 digits are needed (55 < 2^6), but use a small buffer. */
    int digits[32];
    int len = 0;
    seq_t base = (seq_t)n;
    while (val > 0) {
        digits[len++] = (int)(val % base);
        val /= base;
    }

    /* Interpret the base‑n digits as decimal digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + digits[i];

    return result;
}
