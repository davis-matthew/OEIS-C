/*
 * A018482  Divisors of 488.
 * https://oeis.org/A018482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A018482: Divisors of 488 in increasing order.
   Implementation enumerates all divisors from the prime factorisation
   488 = 2^3 * 61^1 and returns the nth divisor (1‑based).  For n > 8
   the function returns 0. */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* 488 = 2^3 * 61^1  →  8 divisors in total. */
    if (n == 0)   /* The sequence is defined only for n ≥ 1. */
        return 0;

    /* For n > 8 no divisor exists; return 0. */
    if (n > 8)
        return 0;

    /* Enumerate the 8 divisors directly.  The inner loop (index a)
       produces 2^a for a = 0..3; the outer loop (index b) toggles the
       factor of 61 (b = 0 or 1).  This order is already sorted. */
    static const seq_t divisors[8] = {
        ((seq_t)1) << 0,  ((seq_t)1) << 1,  ((seq_t)1) << 2,  ((seq_t)1) << 3,
        ((seq_t)61) << 0, ((seq_t)61) << 1, ((seq_t)61) << 2, ((seq_t)61) << 3
    };

    return divisors[n - 1];
}
