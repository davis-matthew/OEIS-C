/*
 * A095510  a(n) = 59 written in base n.
 * https://oeis.org/A095510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the decimal integer formed by the digits of 59 in base n
 *            (with the special case n = 1 giving 59 ones)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) {
        /* 59 ones */
        seq_t v = 0;
        for (idx_t i = 0; i < 59; ++i)
            v = v * 10 + 1;
        return v;
    }

    /* For n >= 2: find the representation of 59 in base n. */
    seq_t tmp = 59;
    seq_t digits[16];   /* enough for any base > 1 */
    int dcnt = 0;

    while (tmp > 0) {
        digits[dcnt++] = tmp % n;
        tmp = tmp / n;
    }

    seq_t res = 0;
    for (int i = dcnt - 1; i >= 0; --i)
        res = res * 10 + digits[i];

    return res;
}
