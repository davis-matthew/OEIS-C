/*
 * A035022  One eighth of 9-factorial numbers.
 * https://oeis.org/A035022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n) = (9*n-1)*a(n-1) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* safety for non‑positive index */

    seq_t res = 1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t factor = 9 * (seq_t)i - 1;
        res *= factor;
    }
    return res;
}
