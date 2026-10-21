/*
 * A095623  a(n) = 116 written in base 10 - n.
 * https://oeis.org/A095623
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = the integer obtained by writing 116 in base (10‑n) and
 *            then interpreting its digits as a decimal number. For n = 0..8
 *            this is the usual base conversion; for n = 9 (base 1) the
 *            representation is 116 ones, i.e. the decimal number with 116
 *            digits equal to 1. For n ≥ 10 the base is non‑positiv...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the base is ≤ 0, the term is undefined; return 0. */
    if (n >= 10)
        return 0;

    /* Handle the unary case (base 1) separately. */
    if (n == 9) {
        seq_t result = 0;
        for (idx_t i = 0; i < 116; ++i) {
            result = result * (seq_t)10 + 1;
        }
        return result;
    }

    /* For bases 2…9: convert 116 to that base, then reconstruct the decimal number. */
    idx_t base = 10 - n;
    idx_t digits[32];   /* enough for any base ≥ 2 (116 is tiny). */
    idx_t kd = 0;
    seq_t tmp = (seq_t)116;

    while (tmp != 0) {
        seq_t d = tmp % (seq_t)base;
        tmp = tmp / (seq_t)base;
        digits[kd++] = (idx_t)d;   /* store least‑significant digit first */
    }

    /* Build the decimal number from the most‑significant digit. */
    seq_t result = 0;
    for (int i = (int)kd - 1; i >= 0; --i) {
        result = result * (seq_t)10 + digits[i];
    }
    return result;
}
