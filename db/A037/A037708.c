/*
 * A037708  Decimal expansion of a(n) is given by the first n terms of the
 *          periodic sequence with initial period 1,3,0,2.
 * https://oeis.org/A037708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A037708
   a(n) is the integer formed by the first n digits of the repeating
   pattern 1,3,0,2.  Recurrence:
       a(1) = 1
       a(n) = 10 * a(n-1) + d,
   where d = {1,3,0,2}[(n-1) % 4]  (n ≥ 2) */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* out‑of‑range, though n ≥ 1 by spec */

    static const seq_t period[4] = { (seq_t)1, (seq_t)3, (seq_t)0, (seq_t)2 };
    seq_t res = 0;
    for (idx_t i = 0; i < n; ++i) {
        res = res * 10 + period[i % 4];
    }
    return res;
}
