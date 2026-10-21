/*
 * A052518  Number of pairs of cycles of cardinality at least 2.
 * https://oeis.org/A052518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 21, u128 n <= 33,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052518: Number of pairs of cycles of cardinality at least 2.
   Recurrence (valid for n ≥ 3):
   n*a(n+2) + (1 - n - 2*n^2)*a(n+1) - n*(1 - n^2)*a(n) = 0
   with a(0) = a(1) = a(2) = a(3) = 0, a(4) = 6.
*/

#include <stdint.h>
#include "../utils/mul_small.h"   /* declares seq_t mul_small(seq_t, unsigned int) */

seq_t a(idx_t n)
{
    /* Handle trivial initial values directly. */
    if (n <= 3) return (seq_t)0;
    if (n == 4) return (seq_t)6;

    /* a_{r}  : a(n) at current step (initially a_3)
       a_{r+1}: a(n+1) at current step (initially a_4)
       a_{r+2}: a(n+2) computed in the loop */
    seq_t a_r   = (seq_t)0;
    seq_t a_r1  = (seq_t)6;
    seq_t a_r2;

    /* Iterate from r = 3 to r = n-2, computing a(r+2) at each step. */
    for (idx_t r = 3; r <= n - 2; ++r)
    {
        /* Small-int coefficients for this r. */
        unsigned int mult1 = (unsigned int)(r + 2 * r * r - 1);   /* n + 2n^2 - 1 */
        unsigned int mult2 = (unsigned int)(r * r - 1);           /* n^2 - 1   */

        if (sizeof(seq_t) == sizeof(uint64_t))
        {
            /* 64‑bit build: use 128‑bit intermediates to avoid overflow. */
            unsigned __int128 term1 = (unsigned __int128)mult1 * (unsigned __int128)a_r1;
            unsigned __int128 term2 = (unsigned __int128)r * (unsigned __int128)mult2 * (unsigned __int128)a_r;
            unsigned __int128 num   = term1 - term2;
            a_r2 = (seq_t)(num / r);
        }
        else
        {
            /* Generic (arbitrary‑precision) build: multiplications are safe. */
            seq_t term1 = mul_small(a_r1, mult1);
            seq_t term2 = mul_small(mul_small(a_r, mult2), r);
            seq_t num   = term1 - term2;
            a_r2 = num / (seq_t)r;
        }

        /* Shift for next iteration. */
        a_r  = a_r1;
        a_r1 = a_r2;
    }

    /* After the loop, a_r1 holds a(n). */
    return a_r1;
}
