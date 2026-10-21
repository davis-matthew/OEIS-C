/*
 * A226274  Position of 1/n in the ordering of the rationals given by
 *          application of the map t -> (1+t,-1/t), cf. A226130.
 * https://oeis.org/A226274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) follows the linear recurrence * a(n) = 5*a(n-1) - 7*a(n-2)
 *            + 4*a(n-3) - a(n-4) for n >= 6 * with initial values * a(1) = 1,
 *            a(2) = 9, a(3) = 31, a(4) = 100, a(5) = 317. * The sequence is
 *            defined for n >= 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)31;
    if (n == 4) return (seq_t)100;
    if (n == 5) return (seq_t)317;

    seq_t a1 = (seq_t)9;   /* a(2) */
    seq_t a2 = (seq_t)31;  /* a(3) */
    seq_t a3 = (seq_t)100; /* a(4) */
    seq_t a4 = (seq_t)317; /* a(5) */
    seq_t an = a4;         /* placeholder for a(n) */

    for (idx_t i = 6; i <= n; ++i) {
        an = 5*a4 - 7*a3 + 4*a2 - a1; /* recurrence */
        a1 = a2; /* shift window */
        a2 = a3;
        a3 = a4;
        a4 = an;
    }
    return an;
}
