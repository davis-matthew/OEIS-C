/*
 * A084472  Write n in binary and replace 0 with 00.
 * https://oeis.org/A084472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = interpret each binary digit of n as decimal, replacing
 *            every 0 by "00". For example, n = 5 (binary 101) → "1 00 1" →
 *            decimal 1001
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n ≥ 1 */
    idx_t msb = 0;         /* index of the most‑significant 1 bit */
    idx_t tmp = n;
    while (tmp >> 1) {     /* find floor(log2(n)) */
        tmp >>= 1;
        ++msb;
    }

    seq_t res = 0;
    for (idx_t i = msb; ; --i) {
        if (n & ((idx_t)1 << i))
            res = res * 10 + 1;   /* append digit '1' */
        else
            res = res * 100;      /* append digits "00" */

        if (i == 0)               /* finished all bits */
            break;
    }
    return res;
}
