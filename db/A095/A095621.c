/*
 * A095621  a(n) = 115 written in base 10 - n.
 * https://oeis.org/A095621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of the decimal number 115 in base (10‑n),
 *            interpreted as a decimal integer. For n = 0…9 this yields: 115,
 *            137, 163, 223, 311, 430, 1303, 11021, 1110011, 115 ones For n >
 *            9 the base is ≤ 1 and the sequence is not defined; 0 is returned
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* constant decimal value 115 */
    const idx_t v = 115;
    /* base from which to write 115; base = 10-n */
    idx_t base = (idx_t)10 - n;
    /* undefined for n > 9 (base ≤ 1) */
    if (n > 9) return 0;

    /* Special case: base 1 (unary).  Produce a number consisting of
       115 decimal 1’s. */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 115; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert 115 to the chosen base, storing the digits. */
    idx_t tmp = v;
    /* Max digits needed: 115 in base 2 has at most 7 digits; buffer size 20 is safe. */
    idx_t digits[20];
    idx_t len = 0;
    while (tmp > 0) {
        digits[len++] = tmp % base;
        tmp /= base;
    }

    /* Build the decimal number from the digits. */
    seq_t result = 0;
    for (idx_t i = len; i > 0; --i) {
        result = result * 10 + digits[i - 1];
    }

    return result;
}
