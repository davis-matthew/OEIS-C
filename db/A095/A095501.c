/*
 * A095501  a(n) = 54 written in base 10 - n.
 * https://oeis.org/A095501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 54 in base (10‑n) interpreted as a
 *            decimal integer
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t base = 10 - n;                    /* base for the representation */
    if (base <= 0)                          /* undefined for base <= 0 */
        return 0;

    seq_t val = 0;
    seq_t temp = 54;
    seq_t digits[60];                      /* enough for n ≤ 9 (max 54 digits for base 1) */
    int dlen = 0;

    if (base == 1) {                        /* unary representation */
        for (idx_t i = 0; i < temp; ++i)
            digits[dlen++] = 1;
    } else {
        /* collect digits least‑significant first */
        while (temp > 0) {
            seq_t rem = temp % (seq_t)base;
            temp = temp / (seq_t)base;
            digits[dlen++] = rem;
        }
    }

    /* convert digits to a decimal number */
    for (int i = dlen - 1; i >= 0; --i)
        val = val * 10 + digits[i];

    return val;
}
