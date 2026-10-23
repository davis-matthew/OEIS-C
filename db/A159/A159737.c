/*
 * A159737  Number of permutations of 6 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 local maxima.
 * https://oeis.org/A159737
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the linear recurrence a(n) = 189*a(n-1) -
 *            10731*a(n-2) + 173215*a(n-3) - 1094856*a(n-4) + 2420208*a(n-5)
 *            with initial values: a(2)=150, a(3)=22680, a(4)=2596608,
 *            a(5)=273322980, a(6)=27558217008 offset: 2 (a(2) is the first
 *            term)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n < 2)
        return 0; /* undefined for n<2, return 0 as safe value */

    /* direct returns for the first five terms */
    if (n == 2) return (seq_t)150;
    if (n == 3) return (seq_t)22680;
    if (n == 4) return (seq_t)2596608;
    if (n == 5) return (seq_t)273322980;
    if (n == 6) return (seq_t)27558217008;

    /* initialise the sliding window with a(2)..a(6) */
    seq_t a0 = (seq_t)150,          /* a(n-5) */
           a1 = (seq_t)22680,       /* a(n-4) */
           a2 = (seq_t)2596608,     /* a(n-3) */
           a3 = (seq_t)273322980,   /* a(n-2) */
           a4 = (seq_t)27558217008; /* a(n-1) */

    /* compute successive terms until we reach a(n) */
    for (idx_t k = 7; k <= n; ++k)
    {
        seq_t next = (seq_t)189   * a4
                   - (seq_t)10731 * a3
                   + (seq_t)173215 * a2
                   - (seq_t)1094856 * a1
                   + (seq_t)2420208 * a0;

        /* slide the window: a(n-5) becomes a(n-4), etc. */
        a0 = a1;
        a1 = a2;
        a2 = a3;
        a3 = a4;
        a4 = next;
    }

    return a4; /* a(n) */
}
