/*
 * A186494  Adjusted joint rank sequence of (f(i)) and (g(j)) with f(i) before
 *          g(j) when f(i)=g(j), where f(i)=5i and g(j)=j-th pentagonal
 *          number. Complement of A186493.
 * https://oeis.org/A186494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*(3*n-1)/10)
 * Range:     u8 n <= 13, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is the rank of the n‑th pentagonal number among the
   multiples of 5 and pentagonal numbers, with ties resolved
   by placing the multiple of 5 before the pentagonal number. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* n is at least 1 in normal use */
    seq_t sn = (seq_t)n;                /* cast idx_t to seq_t */
    seq_t pent = sn * ((sn * 3) - 1) / 2; /* pentagonal number P_n */
    return sn + (pent / 5);             /* rank: n + ⌊P_n/5⌋ */
}
