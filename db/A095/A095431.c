/*
 * A095431  a(n) = 16 written in base n.
 * https://oeis.org/A095431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = decimal integer obtained by writing the decimal number 16
 *            in base n and concatenating its digits in base 10. For n = 1
 *            this is the unary representation (16 ones)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function returns the n-th term of the sequence. */
seq_t a(idx_t n)
{
    /* Base 1: 16 ones. */
    if (n == 1)
        return (seq_t)1111111111111111ULL;   /* 16 digits of '1' */

    /* For n >= 17 the base-n representation of 16 is a single digit 16
       which as a decimal integer is simply 16. */
    if (n >= 17)
        return (seq_t)16;

    /* For 2 <= n <= 16: convert 16 to base n. */
    const seq_t val = (seq_t)16;
    seq_t temp = val;
    int digits[64];
    int len = 0;

    /* Extract digits least‑significant first. */
    while (temp > 0) {
        digits[len++] = (int)(temp % (seq_t)n);
        temp /= (seq_t)n;
    }
    if (len == 0) {          /* should not happen for val = 16 */
        digits[0] = 0;
        len = 1;
    }

    /* Compose the decimal integer by concatenating the digit strings.
       For digits 0–9 we multiply by 10, for digit 16 we multiply by 100
       (the only case >9 that can appear here). */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d = digits[i];
        seq_t pow10 = (d < 10) ? (seq_t)10 : (seq_t)100;
        result = result * pow10 + (seq_t)d;
    }
    return result;
}
