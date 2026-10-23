/*
 * A041495  Denominators of continued fraction convergents to sqrt(264).
 * https://oeis.org/A041495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 130*a(n-2) - a(n-4) for n > 3 Initial values: a(0)=1,
 *            a(1)=4, a(2)=129, a(3)=520
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle first four values directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)129;
    if (n == 3) return (seq_t)520;

    /* ring buffer holding the last four computed terms */
    seq_t prev[4] = { (seq_t)1, (seq_t)4, (seq_t)129, (seq_t)520 };
    idx_t i;

    for (i = 4; i <= n; ++i) {
        /* compute a(i) = 130*a(i-2) - a(i-4) */
        seq_t val = ((seq_t)130 * prev[(i + 2) % 4]) - prev[(i + 0) % 4];
        prev[i % 4] = val;
    }

    return prev[n % 4];
}
