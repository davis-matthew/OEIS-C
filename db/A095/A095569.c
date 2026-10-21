/*
 * A095569  a(n) = 88 written in base 12 - n.
 * https://oeis.org/A095569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal‐digit representation of 88 in base (12-n)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A095569 – a(n) = 88 written in base 12 – n.  The result is the
 *  ordinary decimal integer obtained by writing the digits of the
 *  base‑(12-n) expansion of 88 (decimal) without using any letters,
 *  and treating that digit string as a decimal number.
 *
 *  For n >= 0, base = 12 - n.  If base == 1 the result consists of
 *  eighty‑eight 1's.  For base <= 0 the value is defined as 0.
 */

seq_t a(idx_t n)
{
    int base = (int)(12 - n);

    /* Undefined bases are treated as 0 */
    if (base <= 0) {
        return 0;
    }

    /* Base 1 (unary) – a string of eighty‑eight 1's */
    if (base == 1) {
        seq_t res = 0;
        /* 88 repetitions of digit 1 */
        for (idx_t i = 0; i < (idx_t)88; ++i) {
            res = res * 10 + 1;
        }
        return res;
    }

    /* Convert the decimal number 88 into the given base and build
     * the decimal integer from its digits. */
    unsigned int digits[32];
    int len = 0;
    unsigned int val = 88;            /* constant required by the sequence */

    while (val > 0) {
        digits[len++] = val % base;
        val /= base;
    }

    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i) {
        res = res * 10 + (seq_t)digits[i];
    }

    return res;
}
