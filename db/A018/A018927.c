/*
 * A018927  For each permutation p of {1,2,...,n} define maxjump(p) = max(p(i)
 *          - i); a(n) is sum of maxjumps of all p.
 * https://oeis.org/A018927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*n! - Σ_{k=1}^{n-1} k! * k^{n-k}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   pow_small_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For each n >= 1 returns the n-th term of OEIS A018927. */
#include <stddef.h>
#include "../utils/pow_small_seq.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;           /* not used by OEIS but harmless */

    seq_t fact = 1;          /* factorial of current k   */
    seq_t sum  = 0;          /* Σ k! * k^{n-k} */

    for (idx_t k = 1; k <= n - 1; ++k)
    {
        fact *= k;                         /* k!  */
        seq_t pow_val = pow_small_seq((seq_t)k, n - k);   /* k^{n-k} */
        sum += fact * pow_val;
    }

    seq_t factN = fact * n;                /* n!   */
    seq_t result = (n - 1) * factN - sum;  /* a(n) */
    return result;
}
