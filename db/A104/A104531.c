/*
 * A104531  Expansion of (1+sqrt(1-4*x))/(5*sqrt(1-4*x)-3).
 * https://oeis.org/A104531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 176 (2026-11-03)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A104531
   Recurrence (valid for n ≥ 3):
     4*n*a(n) = (41*n-24)*a(n‑1) - 50*(2*n-3)*a(n‑2)
   Initial values:
     a(0)=1, a(1)=4, a(2)=24
   The function returns the exact value for any n ≥ 0 in the given seq_t type.
   Intermediate products are performed in wide_u to avoid
   overflow on 64‑bit builds; the result is then cast back to seq_t. */
   
#include <stdint.h>

/* intermediate type: 128 bits at the fixed widths, seq_t for big (where
   __int128 would truncate) */
#ifdef __cplusplus
typedef seq_t wide_u;
typedef seq_t wide_s;
#else
typedef unsigned __int128 wide_u;
typedef __int128 wide_s;
#endif
seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)24;

    /* a(i‑2) and a(i‑1) for the current i, starting with i = 3 */
    seq_t prev2 = (seq_t)24; /* a(2)  */
    seq_t prev1 = (seq_t) 4; /* a(1)  */
    seq_t cur;               /* a(i)  */

    for (idx_t i = 3; i <= n; ++i)
    {
        /* Coefficients are small enough to fit in 64 bits for the
           requested range of n (up to 10000).  But the products with
           a(i‑1) and a(i‑2) can overflow 64 bits, so compute in
           wide_u.                                   */
        wide_u coeff1 = (wide_u)(41 * (uint64_t)i - 24);
        wide_u coeff2 = (wide_u)(50 * (2 * (uint64_t)i - 3));
        wide_u prod1 = coeff1 * (wide_u)prev2;
        wide_u prod2 = coeff2 * (wide_u)prev1;
        wide_u numer = prod1 - prod2;
        wide_u denom = (wide_u)(4 * (uint64_t)i);

        cur = (seq_t)(numer / denom);

        /* Shift for next iteration */
        prev1 = prev2;
        prev2 = cur;
    }

    return prev2;  /* a(n) */
}
