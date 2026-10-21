/*
 * A213921  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement: at the beginning filled odd places of layer
 *          clockwise, next - even places clockwise. Table T(n,k) read by
 *          antidiagonals.
 * https://oeis.org/A213921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = i*i - (j mod 2)*i + 2 - floor((j+2)/2) if i>j; else a(n)
 *            = j*j - ((i mod 2)+1)*j + floor((i+3)/2) where (i,j) are the
 *            anti‑diagonal coordinates of n as described in OEIS A213921
 * Range:     u8 n <= 136, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Find t such that t(t+1)/2 < n <= (t+1)(t+2)/2.
       This t is the floor((sqrt(8n-7)-1)/2) used in the OEIS definition. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri < n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t t = low;

    seq_t nS  = (seq_t)n;
    seq_t tS  = (seq_t)t;
    seq_t i = nS - tS * (tS + 1) / 2;
    seq_t jt = (tS * tS + 3 * tS + 4) / 2;
    seq_t j = jt - nS;

    if (i > j) {
        seq_t a = i * i - (j & 1) * i + 2 - ((j + 2) / 2);
        return a;
    } else {
        seq_t a = j * j - (((i & 1) + 1) * j) + ((i + 3) / 2);
        return a;
    }
}
