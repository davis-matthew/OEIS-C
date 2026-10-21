/*
 * A095536  a(n) = 72 written in base n.
 * https://oeis.org/A095536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer formed by concatenating the base‑n digits
 *            of 72
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Special case: base‑1 (unary) */
    if (n == 1) {
        seq_t result = 0;
        for (idx_t i = 0; i < 72; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Compute the base‑n representation of 72, least‑significant digit first. */
    idx_t  tmp  = 72;
    unsigned digits[8];  /* at most 7 digits for base‑2 */
    idx_t  len = 0;
    while (tmp > 0 && len < 8) {
        digits[len++] = (unsigned)(tmp % n);
        tmp /= n;
    }

    /* Assemble the decimal integer from the digits, most‑significant first. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        unsigned d = digits[len - 1 - i];
        if (d < 10)
            result = result * 10 + d;
        else
            result = result * 100 + d;   /* d is 10…72, two decimal digits */
    }
    return result;
}
