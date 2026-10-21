/*
 * A095611  a(n) = 110 written in base 10 - n.
 * https://oeis.org/A095611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal integer whose digits are the base-(10-n)
 *            representation of 110
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Base for representation */
    idx_t base = (idx_t)10 - n;
    if (base <= 0) {
        /* Undefined for bases <= 0; return 0 for safety */
        return (seq_t)0;
    }
    /* Special case: base 1 (unary) – a(n) is 110 copies of the digit '1' */
    if (base == 1) {
        seq_t result = (seq_t)0;
        for (int i = 0; i < 110; ++i) {
            result = result * ((seq_t)10) + ((seq_t)1);
        }
        return result;
    }

    /* Convert 110 (decimal) to the specified base.
       Store digits in reverse order, then construct the decimal integer
       formed by those digits. */
    const seq_t VALUE = (seq_t)110;
    int digits[20];
    int cnt = 0;
    seq_t temp = VALUE;
    while (temp > (seq_t)0) {
        digits[cnt++] = (int)(temp % base);
        temp /= base;
    }

    seq_t result = (seq_t)0;
    for (int i = cnt - 1; i >= 0; --i) {
        result = result * ((seq_t)10) + ((seq_t)digits[i]);
    }
    return result;
}
