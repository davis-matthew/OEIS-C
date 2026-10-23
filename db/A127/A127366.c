/*
 * A127366  Let m = floor(sqrt(n)); if n and m have the same parity, a(n) = n
 *          + m, otherwise a(n) = n - m.
 * https://oeis.org/A127366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn
 * Formula:   a(n) = n + floor(sqrt(n)) if (n & 1) == (floor(sqrt(n)) & 1),
 *            otherwise a(n) = n - floor(sqrt(n))
 * Range:     u8 n <= 240, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* integer square‑root for an unsigned integer of type idx_t */
static idx_t floor_isqrt_u64(idx_t n) {
    idx_t res = 0;
    /* iterate over each bit, starting from the highest */
    for (int i = (int)(sizeof(idx_t) * 8) - 1; i >= 0; --i) {
        idx_t bit = (idx_t)1 << i;
        idx_t temp = res | bit;
        /* avoid overflow by using division */
        if (temp <= n / temp)
            res = temp;
    }
    return res;
}

seq_t a(idx_t n) {
    idx_t m = floor_isqrt_u64(n);          /* floor(sqrt(n))   */
    seq_t sn = (seq_t)n;                   /* n as seq_t       */
    seq_t sm = (seq_t)m;                   /* m as seq_t       */
    if ((n & 1) == (m & 1))
        return sn + sm;                    /* same parity      */
    else
        return sn - sm;                    /* opposite parity  */
}
