/*
 * A002022  In the pile of coconuts problem, the number of coconuts that
 *          remain to be shared equally at the end of the process.
 * https://oeis.org/A002022
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (n-1)^(n+1) - (n-1) if n even, a(n) = (n-1)^n - (n-1) if
 *            n odd
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 27,
 *            big any n
 * Helpers:   a000272_ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000272_ipow.h"

seq_t a(idx_t n) {
    seq_t base = (seq_t)n - 1;
    seq_t power;

    if (n % 2 == 0) {
        power = a000272_ipow(base, n + 1);
    } else {
        power = a000272_ipow(base, n);
    }

    return power - base;
}
