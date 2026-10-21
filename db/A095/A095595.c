/*
 * A095595  a(n) = 102 written in base 12 - n.
 * https://oeis.org/A095595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = (decimal value obtained by writing 102 in base (12 - n))
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base = 12 - n. For n > 11 the base is ≤0, undefined → return 0. */
    if (n > (idx_t)11)
        return (seq_t)0;

    idx_t base = (idx_t)12 - n;

    if (base == (idx_t)1) {
        /* Base 1 (unary): 102 ones. */
        seq_t result = (seq_t)0;
        for (idx_t i = (idx_t)0; i < (idx_t)102; ++i)
            result = result * (seq_t)10 + (seq_t)1;
        return result;
    }

    /* Convert 102 to the current base. */
    seq_t value = (seq_t)102;
    unsigned char digits[16];
    int pos = 0;
    while (value > (seq_t)0) {
        digits[pos++] = (unsigned char)(value % (seq_t)base);
        value /= (seq_t)base;
    }

    /* Reverse digits to form the decimal integer. */
    seq_t result = (seq_t)0;
    for (int i = pos - 1; i >= 0; --i)
        result = result * (seq_t)10 + (seq_t)digits[i];

    return result;
}
