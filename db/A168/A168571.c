/*
 * A168571  a(n) = 25*(5^n - 1)/4.
 * https://oeis.org/A168571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*(5^n - 1)/4 (OEIS A168571)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The recurrence given in OEIS is a(n) = 6*a(n-1) - 5*a(n-2), with a(0)=0, a(1)=25, but the closed form is more efficient here. */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* Compute 5ⁿ.  The big‑int routine used for pow_int can handle arbitrary precision, and for fixed‑width types the value stays in range only for the indices whose results fit in seq_t. */
    seq_t pow5 = pow_int((seq_t)5, n);

    /* 5ⁿ – 1 is always divisible by 4 because 5 ≡ 1 (mod 4).  Dividing before multiplying by 25 avoids overflow in the fixed‑width builds. */
    seq_t t = pow5 - (seq_t)1;
    t /= (seq_t)4;
    t *= (seq_t)25;

    return t;
}
