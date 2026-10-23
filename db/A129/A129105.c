/*
 * A129105  Decimal expansion of 84446888 (see A129106).
 * https://oeis.org/A129105
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) = digit at position (n-7) in the decimal expansion of the
 *            constant 84446888
 * Range:     u8 none, u16 none, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 84446888 has 8 digits; a(8) is the first digit, a(15) is the last. */
    if (n < 8 || n > 15)
        return 0;   /* values outside the defined range are undefined; return 0. */

    /* Compute the digits of 84446888 once. */
    static seq_t digit[8];
    static unsigned char initialized = 0;
    if (!initialized) {
        const seq_t N = (seq_t)84446888;  /* constant value */
        seq_t tmp = N;
        /* Extract digits from least‑significant to most‑significant. */
        for (int i = 7; i >= 0; --i) {
            digit[i] = tmp % (seq_t)10;
            tmp /= (seq_t)10;
        }
        initialized = 1;
    }

    /* n-8 is in the range 0..7, guaranteed by the bounds check above. */
    return digit[(int)(n - 8)];
}
