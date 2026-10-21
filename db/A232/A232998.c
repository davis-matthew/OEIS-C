/*
 * A232998  a(n) = 2^((n+1)^2)*3^(n*(n+1)).
 * https://oeis.org/A232998
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^((n+1)^2) * 3^(n*(n+1))
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow3.h"

/* Compute A232998: a(n) = 2^((n+1)^2) * 3^(n*(n+1)) */
seq_t a(idx_t n)
{
    /* Exponents as unsigned 64‑bit integers (idx_t) */
    idx_t exp_two  = (n + 1) * (n + 1);
    idx_t exp_three = n * (n + 1);

    /* pow2 and pow3 return seq_t.  The natural multiplication operator
       works for both built‑in and arbitrary‑precision seq_t. */
    return pow2(exp_two) * pow3(exp_three);
}
