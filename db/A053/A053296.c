/*
 * A053296  Partial sums of A053295.
 * https://oeis.org/A053296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 32, u64 n <= 78, u128 n <= 171,
 *            big any n
 * Helpers:   binom_n6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_n6.h"

/* Recurrence: a(0)=1, a(1)=8,
   a(n) = a(n-1) + a(n-2) + C(n+6,6)   (n ≥ 2)   */

/* Sequence a(n) = partial sums of A053295.  */
seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    seq_t a_nm2 = (seq_t)1;          /* a(n-2)  */
    seq_t a_nm1 = (seq_t)8;          /* a(n-1)  */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t c = binom_n6(i + 6);   /* C(i+6,6) */
        seq_t a_n  = a_nm1 + a_nm2 + c;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}
