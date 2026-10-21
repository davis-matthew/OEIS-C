/*
 * A095557  a(n) = 82 written in base 11 - n.
 * https://oeis.org/A095557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = representation of 82 in base (11 - n) interpreted in
 *            decimal
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t VALUE = (seq_t)82;

    /* Base b = 11 - n */
    if (n > (idx_t)10)          /* b <= 0 : undefined for this sequence */
        return 0;

    int b = 11 - (int)n;
    if (b == 1)                  /* unary base: 82 ones */
    {
        seq_t res = 0;
        for (idx_t i = 0; i < (idx_t)82; ++i)
            res = res * 10 + 1;
        return res;
    }

    /* Determine digits of VALUE in base b (b >= 2) */
    seq_t tmp = VALUE;
    int digits[32];              /* maximum 32 digits is enough */
    int len = 0;
    while (tmp > 0)
    {
        seq_t rem = tmp % (seq_t)b;
        digits[len++] = (int)rem;
        tmp /= (seq_t)b;
    }

    /* Build the decimal number from the most significant digit */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
        res = res * 10 + (seq_t)digits[i];

    return res;
}
