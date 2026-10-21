/*
 * A234903  a(n)*Pi is the total length of irregular spiral (center points: 1,
 *          3, 2) after n rotations.
 * https://oeis.org/A234903
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-5) - a(n-6) (n >= 6) Initial values: a(0)=0,
 *            a(1)=4, a(2)=11, a(3)=13, a(4)=20, a(5)=24 (Offset 1 as per
 *            OEIS: a(1)=4, a(2)=11, …)
 * Range:     u8 n <= 53, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* handle n = 0 safely (not used by the sequence itself) */
    if (n == 0) return 0;

    /* store the last 6 values; indices 0..5 correspond to a(n) with
       n modulo 6.  The array is updated cyclically. */
    seq_t a_val[6] = {0, 4, 11, 13, 20, 24};

    if (n <= 5) return a_val[n];

    /* compute from a(6) up to a(n) */
    for (idx_t i = 6; i <= n; ++i) {
        seq_t ai = a_val[(i - 1) % 6] + a_val[(i - 5) % 6] - a_val[(i - 6) % 6];
        a_val[i % 6] = ai;
    }

    return a_val[n % 6];
}
