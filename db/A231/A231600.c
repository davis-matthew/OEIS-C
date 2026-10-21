/*
 * A231600  Output of a finite state automaton generating the period doubling
 *          sequence, when fed with binary representation of n, read from
 *          right to left.
 * https://oeis.org/A231600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff the exponent of 2 dividing (n+1) is odd and (n+1)
 *            is not a power of 2; otherwise a(n) = 0. Here v_2(m) denotes the
 *            largest k such that 2^k | m
 * Range:     u8 n <= 6124, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n + 1;           /* m = n+1 */
    idx_t v = 0;                      /* exponent of 2 in m */
    seq_t t = m;
    while ((t & 1) == 0) {            /* while t is even */
        ++v;
        t >>= 1;
    }
    bool power_of_two = (m & (m - 1)) == 0;   /* true if m is 2^k */
    return (seq_t)((v & 1) && !power_of_two);
}
