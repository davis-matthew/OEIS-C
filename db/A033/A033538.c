/*
 * A033538  a(0)=1, a(1)=1, a(n) = 3*a(n-1) + a(n-2) + 1.
 * https://oeis.org/A033538
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 19, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A033538  
   a(0) = 1, a(1) = 1  
   a(n) = 3*a(n-1) + a(n-2) + 1   for n >= 2   */

/* Note: seq_t and idx_t are supplied by the build environment.
   seq_t may be uint8_t … uint64_t, unsigned __int128, or an
   arbitrary‑precision integer class. idx_t is an unsigned integer type. */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;

    /* iterative computation – avoids undefined behaviour and
       guarantees correct arithmetic for every seq_t implementation     */
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = 3 * prev1 + prev2 + 1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
