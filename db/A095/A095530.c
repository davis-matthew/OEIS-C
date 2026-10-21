/*
 * A095530  a(n) = 69 written in base n.
 * https://oeis.org/A095530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 69 written in base n, the digits of the base‑n
 *            representation concatenated as a decimal integer (base‑10)
 * Range:     u8 none, u16 none, u32 none, u64 none, u128 none, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined base, safety net */

    if (n == 1)                     /* unary representation: 69 ones */
    {
        seq_t res = 0;
        for (idx_t i = 0; i < 69; ++i)
            res = res * (seq_t)10 + (seq_t)1;
        return res;
    }

    if (n > 69)                     /* single digit < base, value is 69 itself */
        return (seq_t)69;

    static const seq_t TEN     = (seq_t)10;
    static const seq_t HUNDRED = (seq_t)100;

    /* compute decimal digits of 69 in base n */
    idx_t v = 69;
    int digits[32];
    int len = 0;
    while (v > 0)
    {
        digits[len++] = v % n;
        v /= n;
    }

    /* build the concatenated decimal integer */
    seq_t res = 0;
    for (int i = len - 1; i >= 0; --i)
    {
        int d = digits[i];
        if (d < 10)
            res = res * TEN + (seq_t)d;
        else
            res = res * HUNDRED + (seq_t)d;
    }
    return res;
}
