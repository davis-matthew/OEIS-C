/*
 * A081871  a(1)=1, a(n)=2*a(n-1)+1 if that number is composite, a(n)=a(n-1)+1
 *          otherwise.
 * https://oeis.org/A081871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1 let t=2*a(n-1)+1; if t is composite then a(n)=t,
 *            else a(n)=a(n-1)+1. The sequence is computed iteratively,
 *            testing compositeness by isPrimeGeneric
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 40, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"  /* provides bool isPrimeGeneric(seq_t) */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */
    seq_t val = 1;                /* a(1) = 1 */
    if (n == 1) return val;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* candidate 2*a(n-1)+1 */
        seq_t cand = (val << 1) + 1;   /* use shift for speed, same as val*2+1 */
        if (!isPrimeGeneric(cand))     /* composite? */
            val = cand;                /* a(n) = cand */
        else
            val = val + 1;             /* a(n) = a(n-1)+1 */
    }
    return val;
}
