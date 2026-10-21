/*
 * A084764  a(n) = 2*a(n-1)^2 - 1, a(0)=1, a(1)=4.
 * https://oeis.org/A084764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)^2 - 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(0) = 1, a(1) = 4      */
/* The function recomputes the sequence on each call; no dynamic
   allocation is performed, so it works uniformly for any seq_t
   typedef (plain integer, 128‑bit, or arbitrary‑precision class).  */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Fast path for the first two terms */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* Compute iteratively up to n */
    seq_t prev = (seq_t)4;          /* a(1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        /* next = 2*prev*prev - 1 */
        seq_t sq = prev * prev;
        seq_t next = (seq_t)2 * sq - (seq_t)1;
        prev = next;
    }
    return prev;
}
