/*
 * A073855  Number of steps to reach 0 starting with n and applying the
 *          process x ->bigomega(x), where bigomega = A001222.
 * https://oeis.org/A073855
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(n)=1+a(Ω(n)) for n≥1; Ω(n) is the total number of
 *            prime * factors of n counted with multiplicity (A001222). This
 *            implementation * iterates the bigomega function until 0 is
 *            reached. It uses the provided * bigomega() helper from
 *            utils/bigomega.h
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t steps = 0;
    idx_t cur = n;
    while (cur > 0) {
        ++steps;
        /* bigomega returns seq_t; cast back to idx_t for the next iteration. */
        cur = (idx_t)bigomega(cur);
    }
    return steps;
}
