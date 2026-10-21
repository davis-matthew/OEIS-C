/*
 * A094232  a(n) = 24 written in base n.
 * https://oeis.org/A094232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer obtained by writing 24 in base n and
 *            concatenating the decimal digits of the base‑n digits
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 12, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 1) {                         /* unary representation : 24 ones */
        seq_t v = 0;
        for (idx_t i = 0; i < 24U; ++i)
            v = v * 10 + 1;
        return v;
    }

    /* Convert 24 to base n and store the digits in reverse order. */
    idx_t tmp = 24;
    int digits[32];
    int len = 0;
    while (tmp > 0U) {
        digits[len++] = tmp % n;           /* least significant digit first */
        tmp /= n;
    }

    /* Build the decimal number by concatenating the decimal
       representations of the collected digits. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        /* compute 10^(number of decimal digits of d) */
        int pow10 = 1;
        int dd = d;
        do {
            pow10 *= 10;
            dd /= 10;
        } while (dd > 0);
        result = result * pow10 + d;
    }
    return result;
}
