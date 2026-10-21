/*
 * A067399  Number of divisors of n in OR-numbral arithmetic.
 * https://oeis.org/A067399
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = # of integers d (1 ≤ d ≤ n) such that there exists e with
 *            n = OR_{i∈bits(e)} (d << i) (OR-numbral arithmetic in base 2)
 * Range:     u8 n <= 2045, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, but safe guard */
    uint64_t u = n;                /* work in native 64‑bit integers   */
    int msbn = 63 - __builtin_clzll(u);   /* highest set bit of n */
    uint64_t cnt = 0;

    for (uint64_t d = 1; d <= u; ++d) {
        uint64_t ud = d;
        int msbd = 63 - __builtin_clzll(ud);          /* highest bit of d */
        int max_shift = msbn - msbd;                 /* maximal shift index */

        uint64_t mask = 0;
        for (int s = 0; s <= max_shift; ++s) {
            uint64_t shifted = ud << s;              /* d shifted by s */
            if ((shifted & ~u) == 0) {              /* no bit outside n */
                mask |= shifted;                     /* accumulate covered bits */
            }
        }
        if (mask == u) ++cnt;                       /* d divides n */
    }
    return (seq_t)cnt;
}
