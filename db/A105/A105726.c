/*
 * A105726  Involution of nonnegative integers. A deeply recursive variant of
 *          A056539.
 * https://oeis.org/A105726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look, base
 * Formula:   a(n) is defined recursively by * If n = 0 or 1, a(n) = n. *
 *            Else, let the binary representation of n have run‐lengths *
 *            r[0], r[1], …, r[k−1] (starting with a 1). Define * s[i] =
 *            a(r[k−1−i]) for i = 0..k−1. * Then a(n) is the binary number
 *            whose run‐lengths are s[0], …, s[k−1] * (again starting w...
 * Range:     u8 n <= 65, u16 n <= 1025, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* memoisation for small arguments */
    static bool done[65];
    static seq_t cache[65];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < 65; ++i) done[i] = false;
        init = true;
    }
    if (n <= 1) return (seq_t)n;
    if (n < 65 && done[n]) return cache[n];

    /* find the binary length of n */
    int bitlen = 0;
    idx_t tmp = n;
    while (tmp) { ++bitlen; tmp >>= 1; }

    /* collect run lengths */
    idx_t runs[70];
    int r = 0;
    int curr = 1;
    int cnt = 0;

    for (int pos = bitlen - 1; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit == curr) {
            ++cnt;
        } else {
            runs[r++] = cnt;
            cnt = 1;
            curr = bit;
        }
    }
    runs[r++] = cnt;              /* last run */

    /* rebuild the number from the reversed and mapped run lengths */
    seq_t val = 0;
    int bit = 1;                  /* start with 1 */
    for (int i = r - 1; i >= 0; --i) {
        seq_t len = a((idx_t)runs[i]);   /* recursive call */

        /* append len bits of ‘bit’ to val */
        for (seq_t k = 0; k < len; ++k) {
            val <<= 1;
            val |= (seq_t)bit;
        }
        bit ^= 1;                 /* flip the bit */
    }

    if (n < 65) { cache[n] = val; done[n] = true; }
    return val;
}
