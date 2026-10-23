/*
 * A040303  Continued fraction for sqrt(321).
 * https://oeis.org/A040303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A040303: continued fraction for sqrt(321). 
   a(0) = 17;  for n > 0, the terms repeat in the cycle
   {1, 10, 1, 34}, i.e. a(n) = cycle[(n-1) mod 4] for n ≥ 1. */

#include <stddef.h>   /* for size_t */

/* Return the nth term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;

    /* The periodic part of the continued fraction expansion of sqrt(321). */
    static const seq_t cycle[4] = {
        (seq_t)1,  /* a(1) */
        (seq_t)10, /* a(2) */
        (seq_t)1,  /* a(3) */
        (seq_t)34  /* a(4) */
    };

    /* Compute the index in the cycle.  (n-1) is guaranteed non‑negative
       because the n == 0 case was handled above. */
    return cycle[(size_t)((n - 1) % 4)];
}
