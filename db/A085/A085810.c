/*
 * A085810  Number of three-choice paths along a corridor of height 5,
 *          starting from the lower side.
 * https://oeis.org/A085810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3) Initial terms: a(1)=1,
 *            a(2)=2, a(3)=5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for uint64_t, idx_t is this type in the build */

seq_t a(idx_t n)
{
    /* Handle the base cases */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)5;

    /* a1 = a(k-1), a2 = a(k-2), a3 = a(k-3) for the current k */
    seq_t a1 = (seq_t)5;   /* a(3) */
    seq_t a2 = (seq_t)2;   /* a(2) */
    seq_t a3 = (seq_t)1;   /* a(1) */

    for (idx_t k = 4; k <= n; ++k) {
        /* compute a(k) = 4*a1 - 3*a2 - a3 */
        seq_t cur = (seq_t)4 * a1;
        cur -= (seq_t)3 * a2;
        cur -= a3;

        /* shift the window for the next iteration */
        a3 = a2;
        a2 = a1;
        a1 = cur;
    }

    return a1;   /* after the loop a1 == a(n) */
}
