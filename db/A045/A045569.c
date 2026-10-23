/*
 * A045569  Numbers whose factorial has '92' as its final two digits before
 *          the trailing zeros.
 * https://oeis.org/A045569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A045569: Numbers m such that m! has trailing zeros removed and the last two digits are 92. */
/* The function searches m incrementally, keeping the factorial with trailing zeros stripped
   and using only integer arithmetic. */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    idx_t count = 0;            /* how many suitable m have been found so far   */
    seq_t f = 1;                /* current value of m! with trailing zeros removed */
    const seq_t MOD = (seq_t)1000000000000ULL;  /* keep enough low‑order digits   */

    for (idx_t m = 1; ; ++m) {
        f *= (seq_t)m;          /* multiply by the next integer          */
        while ((f % 10) == 0)   /* strip trailing zeros after each step  */
            f /= 10;
        f %= MOD;               /* keep the number small                */

        if ((f % 100) == 92) {  /* check the last two digits after stripping */
            ++count;
            if (count == n)     /* desired nth term found                 */
                return (seq_t)m;
        }
    }
}
