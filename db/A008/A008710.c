/*
 * A008710  13 in base 13-n.
 * https://oeis.org/A008710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  fini, nonn, base, full
 * Formula:   a(n) = representation of 13 in base (13-n)
 * Range:     u8 n <= 10, u16 n <= 11, u32 n <= 11, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* If the index is beyond the defined range, return 0 */
    if (n >= 13) return 0;

    idx_t base = 13 - n;
    /* Base 1 -> unary representation (13 ones) */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 13; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* Base >= 2 */
    seq_t digits[32];  // sufficient for all cases
    idx_t dcnt = 0;
    seq_t temp = (seq_t)13;
    seq_t b = (seq_t)base;

    while (temp > (seq_t)0) {
        seq_t r = temp % b;
        digits[dcnt++] = r;
        temp = temp / b;
    }

    seq_t result = 0;
    for (idx_t i = dcnt; i > 0; --i)
        result = result * (seq_t)10 + digits[i - 1];

    return result;
}
