/*
 * A095453  a(n) = 30 written in base 15 - n.
 * https://oeis.org/A095453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the integer 30 in base (15 - n)
 * Range:     u8 n <= 11, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Only defined for n = 0 .. 14 (base 15 down to base 1) */
    if (n > 14) return 0;

    idx_t base = 15 - n;          /* base in which 30 is represented */
    seq_t result = 0;

    /* Unary (base 1) representation: 30 ones. */
    if (base == 1) {
        for (int i = 0; i < 30; ++i)
            result = result * 10 + 1;
        return result;
    }

    /* Convert 30 to the given base (base ∈ [2,15]) */
    uint32_t tmp = 30;
    int digits[10];
    int len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % (uint32_t)base;
        tmp /= (uint32_t)base;
    }

    /* Build the decimal number by concatenating the base‑digits */
    for (int i = len - 1; i >= 0; --i)
        result = result * 10 + (seq_t)digits[i];

    return result;
}
