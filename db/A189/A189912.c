/*
 * A189912  Extended Motzkin numbers, Sum_{k>=0} C(n,k)*C(k), where C(k) is
 *          the extended Catalan number A057977(k).
 * https://oeis.org/A189912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A189912: Extended Motzkin numbers
   a(n) = Σ_{k=0..n} binomial(n,k) * C(k)
   where C(k) = Catalan(k/2) if k even,
                binomial(k,⌊k/2⌋) if k odd
   (C(k) is OEIS A057977(k)) */
#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    seq_t sum = (seq_t)0;
    seq_t bin = (seq_t)1;          /* binomial(n,0) */

    for (idx_t k = (idx_t)0; k <= n; ++k)
    {
        seq_t cat;
        if (k & (idx_t)1)          /* odd k */
        {
            idx_t mid = k >> 1;    /* floor(k/2) */
            cat = (seq_t)1;
            for (idx_t i = (idx_t)1; i <= mid; ++i)
            {
                cat = cat * (k - i + (idx_t)1) / i;
            }
        }
        else                      /* even k */
        {
            cat = catalan(k >> 1);
        }

        sum += bin * cat;

        if (k == n) break;        /* last term, no update needed */

        /* update binomial coefficient: bin(n,k+1) from bin(n,k) */
        bin = bin * (n - k) / (k + (idx_t)1);
    }

    return sum;
}
