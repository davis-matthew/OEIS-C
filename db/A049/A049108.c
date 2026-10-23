/*
 * A049108  a(n) is the number of iterations of Euler phi function needed to
 *          reach 1 starting at n (n is counted).
 * https://oeis.org/A049108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(phi(n)) + 1 with a(1) = 1 (number of iterations of
 *            Euler's totient function needed to reach 1, counting n itself)
 * Range:     u8 n <= 1809, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   calc_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    /* n >= 1 as per OEIS offset. For safety return 0 if n == 0. */
    if (n == 0)
        return 0;
    seq_t steps = 1;          /* count the starting number */
    seq_t m = (seq_t)n;       /* use seq_t for intermediate values */

    while (m != 1) {
        m = calc_phi(m);      /* Euler's totient function */
        ++steps;
    }
    return steps;
}
