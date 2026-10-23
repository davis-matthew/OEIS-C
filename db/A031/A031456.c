/*
 * A031456  Numbers whose base-3 representation has 4 more 0's than 2's.
 * https://oeis.org/A031456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base‑3 representation has 4 more 0’s than 2’s. */
#include <stdbool.h>

/* Helper: test whether a number satisfies the condition. */
static inline bool ok(seq_t x)
{
    uint64_t cnt0 = 0, cnt2 = 0;          /* counts fit in 64 bits */
    seq_t tmp = x;
    while (tmp > 0) {
        seq_t d = tmp % 3;
        if (d == 0) cnt0++;
        else if (d == 2) cnt2++;
        tmp = tmp / 3;
    }
    return cnt0 == cnt2 + 4;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for n<1, but return 0 */
    idx_t found = 0;
    seq_t v = 81;                /* first term is 3^4 = 81 */
    while (1) {
        if (ok(v)) {
            ++found;
            if (found == n) return v;
        }
        v = v + 1;
    }
}
