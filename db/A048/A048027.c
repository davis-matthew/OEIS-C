/*
 * A048027  Number of nonempty subsets of {1,2,...,n} in which exactly 1/2 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{min(M,n-M)} C(M,k)*C(n-M,k), where M =
 *            floor((n-3)/3)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include for mul_small helper (big‑int support) */
#include "../utils/mul_small.h"

/* Compute A048027 for index n (1‑based) */
seq_t a(idx_t n)
{
    /* Small set size M = floor((n-3)/3) */
    idx_t M = (n >= 3 ? (n - 3) / 3 : 0);
    idx_t big = n - M;                       /* Size of the large set */
    idx_t k_max = (M < big) ? M : big;       /* Upper bound for k */

    seq_t sum = 0;
    if (k_max == 0)
        return sum;

    seq_t bin1 = 1;  /* C(M,0) */
    seq_t bin2 = 1;  /* C(big,0) */

    for (idx_t k = 1; k <= k_max; ++k)
    {
        if (sizeof(seq_t) <= 8)        /* 64‑bit or smaller build */
        {
            unsigned __int128 t1 = (unsigned __int128)bin1 * (M - k + 1);
            bin1 = (seq_t)(t1 / k);

            unsigned __int128 t2 = (unsigned __int128)bin2 * (big - k + 1);
            bin2 = (seq_t)(t2 / k);
        }
        else                           /* arbitrary‑precision build */
        {
            bin1 = mul_small(bin1, (unsigned int)(M - k + 1));
            bin1 /= k;

            bin2 = mul_small(bin2, (unsigned int)(big - k + 1));
            bin2 /= k;
        }

        sum += bin1 * bin2;
    }

    return sum;
}
