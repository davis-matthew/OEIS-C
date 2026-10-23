/*
 * A162672  Lunar product 19*n.
 * https://oeis.org/A162672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = lunar product of 19 and n computed by taking the
 *            digitwise minimum of each pair of digits and then, for each
 *            position, keeping the maximum of all such minima
 * Range:     u8 n <= 99, u16 n <= 9999, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    /* Extract decimal digits of n (least significant first). */
    uint8_t nd[25];                       /* enough for 64‑bit n */
    int len = 0;
    idx_t tmp = n;
    do {
        nd[len++] = (uint8_t)(tmp % 10);
        tmp /= 10;
    } while (tmp > 0);

    /* Result digits (least significant first). */
    uint8_t res[25] = {0};

    /* Digits of the constant 19: [9,1] (units, tens). */
    for (int i = 0; i < 2; ++i) {
        uint8_t ai = (i == 0) ? 9 : 1;
        for (int j = 0; j < len; ++j) {
            uint8_t bj = nd[j];
            uint8_t mimin = (ai < bj) ? ai : bj;   /* min(a_i,b_j) */
            int pos = i + j;
            if (mimin > res[pos]) res[pos] = mimin;
        }
    }

    /* Find the most significant non‑zero digit. */
    int hi = len + 1;                     /* max possible index */
    while (hi > 0 && res[hi] == 0) --hi;
    if (hi < 0) return 0;                 /* should not happen except n==0 */

    /* Convert digit array back into a seq_t value. */
    seq_t val = 0;
    for (int k = hi; k >= 0; --k) {
        val = val * 10 + res[k];
    }
    return val;
}
