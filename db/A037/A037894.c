/*
 * A037894  a(n)=(1/2)*Sum{|d(i)-e(i)|} where Sum{d(i)*8^i} is the base 8
 *          representation of n and e(i) are the digits d(i) in reverse order.
 * https://oeis.org/A037894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1/2 * Σ_i |d(i) – d(L‑1‑i)|
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where d(i) are the base‑8 digits of n (least significant first). */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* for n == 0 (outside the stated offset) return 0 */
    if (n == 0)
        return 0;

    /* store base‑8 digits of n */
    unsigned char d[64];
    idx_t len = 0;
    idx_t tmp = n;
    while (tmp) {
        d[len++] = (unsigned char)(tmp & 7u);   /* tmp % 8 */
        tmp >>= 3;                              /* tmp /= 8 */
    }
    if (len == 0)            /* n == 0 */
        d[len++] = 0;

    /* sum absolute differences between symmetric digits */
    seq_t sum = 0;
    for (idx_t i = 0; i < len / 2; ++i) {
        int diff = (int)d[i] - (int)d[len - 1 - i];
        if (diff < 0)
            diff = -diff;
        sum += (seq_t)diff;
    }
    return sum;
}
