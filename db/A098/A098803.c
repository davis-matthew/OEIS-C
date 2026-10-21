/*
 * A098803  a(n) = n^7 * 7^n.
 * https://oeis.org/A098803
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 * 7^n
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 13, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* a(0) = 0 */

    /* compute 7^n */
    seq_t pow7 = 1;
    seq_t base = 7;
    idx_t exp = n;
    while (exp) {
        if (exp & 1U) pow7 *= base;
        base *= base;
        exp >>= 1U;
    }

    /* compute n^7 */
    seq_t sn = (seq_t)n;
    seq_t n7 = 1;
    for (int i = 0; i < 7; ++i) n7 *= sn;   /* 7 multiplications */

    /* return product */
    return pow7 * n7;
}
