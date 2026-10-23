/*
 * A008716  19 in base 19-n.
 * https://oeis.org/A008716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = decimal number obtained by writing 19 in base (19 − n)
 *            (base 1 is represented by 19 consecutive 1's; bases ≤0 are
 *            undefined and produce 0)
 * Range:     u8 n <= 16, u16 n <= 17, u32 n <= 17, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* sequence ends after n=18 (base = 1).  For n≥19 return 0. */
    if (n > (idx_t)18) {
        return (seq_t)0;
    }

    /* base = 19 - n */
    seq_t base = (seq_t)(19U - n);

    /* Special case: base == 1 -> 19 ones */
    if (base == (seq_t)1) {
        seq_t result = 0;
        for (idx_t i = 0; i < (idx_t)19; ++i) {
            result = result * (seq_t)10 + (seq_t)1;
        }
        return result;
    }

    /* For base <= 0 (should not occur for n <= 18) */
    if (base <= (seq_t)0) {
        return (seq_t)0;
    }

    /* Convert 19 to base `base` (base ≥ 2).  digits are stored least‑significant first. */
    seq_t digits[32];
    int dcnt = 0;
    seq_t value = (seq_t)19;
    while (value > (seq_t)0) {
        digits[dcnt++] = value % base;
        value = value / base;
    }

    /* Build the decimal integer from the digit vector. */
    seq_t result = (seq_t)0;
    for (int i = dcnt - 1; i >= 0; --i) {
        result = result * (seq_t)10 + digits[i];
    }
    return result;
}
