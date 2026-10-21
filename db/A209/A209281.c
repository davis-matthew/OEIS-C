/*
 * A209281  Start with first run [0,1] then, for n >= 2, the n-th run has
 *          length 2^n and is the concatenation of [a(1),a(2),...,a(2^n/2)]
 *          and [n-a(1),n-a(2),...,n-a(2^n/2)].
 * https://oeis.org/A209281
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, changed
 * Formula:   a(n) = a(⌈n/2⌉) + T(n−1), a(1) = 0 where T(m) = 1 if m has odd
 *            number of 1‑bits, else 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline int parity_idx(idx_t n)
{
    int p = 0;
    while (n) {          /* toggle parity for each set bit */
        p ^= 1;
        n &= n - 1;      /* clear the lowest set bit */
    }
    return p;
}

seq_t a(idx_t n)
{
    if (n == 1)          /* base case */
        return 0;
    idx_t half = (n + 1) >> 1;              /* ceil(n/2) */
    seq_t half_val = a(half);                /* recursive call */
    int t = parity_idx(n - 1);               /* T(n‑1) */
    return half_val + (seq_t)t;
}
