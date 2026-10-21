/*
 * A087165  a(n)=1 when n == 1 (mod 4), otherwise a(n) = a(n - ceiling(n/4)) +
 *          1. Removing all the 1's results in the original sequence with
 *          every term incremented by 1.
 * https://oeis.org/A087165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1, if n ≡ 1 (mod 4); otherwise a(n) = a(n – ceil(n/4)) +
 *            1. This implementation evaluates the recurrence iteratively,
 *            avoiding any dynamic allocation so that seq_t can be an
 *            arbitrary‑precision integer class in C++
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* safety for out‑of‑range */

    seq_t res = (seq_t)1;                    /* base value a(1) = 1 */

    /* Iterate until n becomes 1 (i.e. n ≡ 1 (mod 4)) */
    while ( (n & (idx_t)3) != (idx_t)1 ) {
        idx_t ceil_div = (n + (idx_t)3) >> 2; /* ceil(n/4) */
        n -= ceil_div;                      /* n = n – ceil(n/4) */
        res++;                              /* +1 in the recurrence */
    }

    return res;
}
