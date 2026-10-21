/*
 * A097775  Pell equation solutions (14*a(n))^2 - 197*b(n)^2 = -1 with b(n) =
 *          A097776(n), n >= 0.
 * https://oeis.org/A097775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 786*a(n-1) - a(n-2), a(0) = 1, a(1) = 787
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for idx_t (uint64_t) */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)787;

    seq_t a0 = (seq_t)1;   /* a(0) */
    seq_t a1 = (seq_t)787; /* a(1) */
    seq_t an = a1;         /* placeholder */

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)786 * a1 - a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}
