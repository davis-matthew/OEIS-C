/*
 * A095631  a(n) = 120 written in base 10 - n.
 * https://oeis.org/A095631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = write 120 (decimal) in base (10 – n) and interpret * the
 *            resulting digit string as a decimal integer. * For example 120
 *            in base 9 is 1 4 3 → 143. * For base 1 (when n = 9) the
 *            representation is 120 repetitions of the digit 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle only defined range 0 ≤ n ≤ 10. For n > 10 the sequence is not defined. */
    if (n > 10) return 0;

    int base = 10 - (int)n;

    if (base > 1) {
        /* Compute the digits of 120 in the given base (least‑to‑most). */
        int val = 120;
        int digits[20];
        int cnt = 0;

        while (val > 0) {
            digits[cnt++] = val % base;
            val /= base;
        }

        /* Build the decimal number by reading digits most‑to‑least. */
        seq_t result = 0;
        for (int i = cnt - 1; i >= 0; --i) {
            result = result * 10 + (seq_t)digits[i];
        }
        return result;
    } else if (base == 1) {
        /* Unary representation: 120 consecutive 1 digits. */
        seq_t result = 0;
        for (int i = 0; i < 120; ++i) {
            result = result * 10 + 1;
        }
        return result;
    } else {
        /* Undefined base (0 or negative). */
        return 0;
    }
}
