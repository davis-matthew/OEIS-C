/*
 * A132961  Total number of all distinct cycle sizes in all permutations of
 *          [n].
 * https://oeis.org/A132961
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{ℓ=1}^n ( n! – g_ℓ(n) ) where g_ℓ(n) = Σ_{j=0}^{⌊n/ℓ⌋}
 *            (–1)^j · n! / (ℓ^j · j!) (the number of permutations of [n] that
 *            contain no ℓ‑cycle)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers needed for idx_t and seq_t */
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;   /* defined only for n ≥ 1 */

    /* compute n! */
    seq_t fac = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n; ++i)
        fac = fac * (seq_t)i;

    seq_t result = (seq_t)0;

    for (idx_t l = (idx_t)1; l <= n; ++l)
    {
        idx_t max_j = n / l;
        seq_t curr = fac;

        /* sums of terms with even and odd j */
        seq_t even_sum = (seq_t)0;
        seq_t odd_sum  = (seq_t)0;

        /* j = 0 term (always positive) */
        even_sum += curr;

        for (idx_t j = (idx_t)1; j <= max_j; ++j)
        {
            curr = curr / (seq_t)l;    /* divide by ℓ^j */
            curr = curr / (seq_t)j;    /* divide by j! */

            if ((j & (idx_t)1) == (idx_t)0)   /* even j */
                even_sum += curr;
            else
                odd_sum  += curr;
        }

        /* number of permutations with no ℓ‑cycle */
        seq_t g_l = even_sum;
        g_l -= odd_sum;                 /* even_sum ≥ odd_sum */

        /* permutations that have at least one ℓ‑cycle */
        seq_t f_l = fac;
        f_l -= g_l;                     /* fac ≥ g_l */

        result += f_l;
    }

    return result;
}
