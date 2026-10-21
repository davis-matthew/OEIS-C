/*
 * A222234  In the number n, replace all (decimal) digits '3' with '4' and
 *          vice versa.
 * https://oeis.org/A222234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   replace all decimal digits 3 with 4 and 4 with 3 in n
 * Range:     u8 n <= 255, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the special case n == 0 */
    if (n == 0)
        return (seq_t)0;

    /* maximum 20 digits for a 64‑bit unsigned integer */
    unsigned char digs[20];
    int cnt = 0;
    idx_t tmp = n;
    /* decompose n into decimal digits (least‑significant first) */
    while (tmp) {
        digs[cnt++] = tmp % 10;
        tmp /= 10;
    }

    seq_t res = 0;
    /* rebuild the number from most‑significant digit, swapping 3 ↔ 4 */
    for (int i = cnt - 1; i >= 0; --i) {
        unsigned char d = digs[i];
        if (d == 3)
            d = 4;
        else if (d == 4)
            d = 3;
        res = res * 10 + d;
    }
    return res;
}
