/*
 * A104531  Expansion of (1+sqrt(1-4*x))/(5*sqrt(1-4*x)-3).
 * https://oeis.org/A104531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
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
   Intermediate products are performed in unsigned __int128 to avoid
   overflow on 64‑bit builds; the result is then cast back to seq_t. */
   
#include <stdint.h>
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
           unsigned __int128.                                   */
        unsigned __int128 coeff1 = (unsigned __int128)(41 * (uint64_t)i - 24);
        unsigned __int128 coeff2 = (unsigned __int128)(50 * (2 * (uint64_t)i - 3));
        unsigned __int128 prod1 = coeff1 * (unsigned __int128)prev2;
        unsigned __int128 prod2 = coeff2 * (unsigned __int128)prev1;
        unsigned __int128 numer = prod1 - prod2;
        unsigned __int128 denom = (unsigned __int128)(4 * (uint64_t)i);

        cur = (seq_t)(numer / denom);

        /* Shift for next iteration */
        prev1 = prev2;
        prev2 = cur;
    }

    return prev2;  /* a(n) */
}
