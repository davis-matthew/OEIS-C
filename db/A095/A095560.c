/*
 * A095560  a(n) = 84 written in base n.
 * https://oeis.org/A095560
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A095560.
   a(n) = representation of the decimal number 84 in base n.
   The return value is the decimal integer obtained by concatenating
   the base‑n digits of 84. For n = 1 (unary), the result is the
   integer consisting of 84 ones. */

seq_t a(idx_t n)
{
    /* Unary expansion: 84 in base 1 is 84 copies of the digit 1. */
    if (n == 1) {
        seq_t val = 0;
        for (idx_t i = 0; i < (idx_t)84; ++i)
            val = val * (seq_t)10 + (seq_t)1;
        return val;
    }

    /* Convert 84 to base n and concatenate the digits in decimal. */
    seq_t temp = (seq_t)84;
    seq_t sn   = (seq_t)n;           /* base as seq_t for arithmetic */

    int digits[64];                  /* enough for any base >= 2 */
    int len = 0;

    /* Extract digits least‑significant first. */
    while (temp > (seq_t)0) {
        digits[len++] = (int)(temp % sn);
        temp = temp / sn;
    }

    /* Build the decimal integer from most‑significant digit. */
    seq_t result = (seq_t)0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * (seq_t)10 + (seq_t)digits[i];
    }

    return result;
}
