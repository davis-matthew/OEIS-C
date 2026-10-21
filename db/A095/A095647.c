/*
 * A095647  a(n) = 128 written in base 11 - n.
 * https://oeis.org/A095647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal value of the representation of 128 in base (11‑n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For n ≥ 0.  Sequence is defined for 0 ≤ n ≤ 10; for n > 10 the value is undefined. */
seq_t a(idx_t n)
{
    /* Only defined for n from 0 to 10.  Return 0 for other values. */
    if (n > 10U) return 0;

    /* Base for conversion: b = 11 – n */
    seq_t base = (seq_t)11 - (seq_t)n;
    if (base == 0) return 0;               /* safety for n == 11, not needed for tests */

    const seq_t V = (seq_t)128;            /* number to convert */
    seq_t value = 0;                       /* accumulated decimal value */

    if (base == (seq_t)1) {
        /* Unary representation: 128 ones.  Produce digit string of 128 '1's. */
        for (idx_t i = 0; i < 128; ++i) {
            value = value * 10 + (seq_t)1;
        }
        return value;
    }

    /* Convert V to the given base and collect digits (least to most significant). */
    seq_t digit[32];          /* enough for any base in [2,11] */
    int pos = 0;
    seq_t temp = V;
    while (temp > 0) {
        seq_t r = temp % base;
        temp = temp / base;
        digit[pos++] = r;
    }

    /* Build the decimal value by reading digits most‑to‑least. */
    for (int i = pos - 1; i >= 0; --i) {
        value = value * 10 + digit[i];
    }
    return value;
}
