/*
 * A095577  a(n) = 92 written in base 15 - n.
 * https://oeis.org/A095577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal obtained from writing 92 in base (15-n)
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle indices beyond the defined range. */
    if (n > 14ULL)
        return (seq_t)0;

    idx_t base = 15 - n;
    if (base == 0)
        return (seq_t)0;

    /* Unary base: 92 in base‑1 is 92 copies of the digit ‘1’. */
    if (base == 1) {
        seq_t res = 0;
        for (idx_t i = 0; i < 92; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    /* Convert 92 to base ‘base’ and build the decimal representation. */
    const idx_t num = 92;
    unsigned int digits[8];   /* 92 needs at most seven digits in base 2. */
    unsigned int cnt = 0;
    idx_t tmp = num;

    while (tmp > 0) {
        digits[cnt++] = (unsigned int)(tmp % base);
        tmp /= base;
    }

    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i)
        res = res * (seq_t)10 + (seq_t)digits[i];

    return res;
}
