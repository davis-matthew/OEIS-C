/*
 * A008715  18 in base 18-n.
 * https://oeis.org/A008715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 16, u32 n <= 16, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A008715: 18 in base (18‑n) */
seq_t a(idx_t n)
{
    /* n is unsigned.  The sequence is defined for 0 ≤ n ≤ 17.  */
    if (n > 17u) return 0;          /* undefined indices return 0 */

    idx_t b = 18u - n;              /* the base */

    /* Unary base (b == 1): 18 ones */
    if (b == 1u) {
        seq_t val = 0;
        for (idx_t i = 0; i < 18; ++i)
            val = val * 10 + 1;
        return val;
    }

    /* Base 0 or negative are not defined (handled above) */
    if (b <= 1u) return 0;

    /* Convert 18 to base b, collecting digits in reverse order. */
    idx_t temp = 18u;
    int digits[20];
    int dsize = 0;
    while (temp > 0) {
        digits[dsize++] = (int)(temp % b);
        temp /= b;
    }

    /* Build the decimal number from the digits. */
    seq_t val = 0;
    for (int i = dsize - 1; i >= 0; --i)
        val = val * 10 + digits[i];

    return val;
}
