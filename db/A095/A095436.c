/*
 * A095436  a(n) = 21 written in base n.
 * https://oeis.org/A095436
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = representation of the decimal number 21 written in base n
 *            (the digits are interpreted as decimal digits). For example,
 *            a(3)=210 because 21_base10 = 210_base3
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 n <= 10, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: n is an idx_t (unsigned 64‑bit integer), a returns a seq_t
   (unsigned integer type of the build or an arbitrary‑precision class).
   The implementation uses only integer arithmetic allowed in the
   build. */

seq_t a(idx_t n)
{
    /* Special case base 1: 21 is written as 21 ones (unary). */
    if (n == 1) {
        seq_t res = 0;
        /* Repeat 21 times: res = res*10 + 1 */
        for (idx_t i = 0; i < 21; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Compute the digits of 21 in base n (least significant first). */
    seq_t val = 21;
    unsigned digits[64];   /* maximum 21 digits for base 1, safe for others */
    int dcnt = 0;
    while (val != 0) {
        unsigned dig = (unsigned)(val % (seq_t)n);
        digits[dcnt++] = dig;
        val = val / (seq_t)n;
    }

    /* Assemble the decimal number from the most significant digit. */
    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        res = res * 10 + digits[i];
    return res;
}
