/*
 * A095537  a(n) = 72 written in base 14 - n.
 * https://oeis.org/A095537
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = decimal concatenation of the base-(14-n) representation
 *            of 72
 * Range:     u8 n <= 9, u16 n <= 11, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n ≥ 14 the base falls below 1 – undefined; return 0 */
    if (n >= 14u) {
        return 0;
    }

    int base = 14 - (int)n;          // base 14 to 1

    if (base == 1) {
        /* Unary case: 72 in base‑1 is 72 ones */
        seq_t ans = 0;
        for (idx_t i = 0; i < 72; ++i) {
            ans = ans * 10 + 1;
        }
        return ans;
    }

    /* Convert 72 to base `base`; store digits LSB->MSB */
    uint32_t digs[64];
    int cnt = 0;
    uint32_t v = 72;
    while (v > 0) {
        digs[cnt++] = v % base;
        v /= base;
    }

    seq_t res = 0;
    for (int i = cnt - 1; i >= 0; --i) {
        uint32_t d = digs[i];
        seq_t mult = (d < 10 ? (seq_t)10 : (seq_t)100);
        res = res * mult + (seq_t)d;
    }
    return res;
}
