/*
 * A097192  Main diagonal of triangle A097190, in which the n-th row
 *          polynomial R_n(y) is formed from the initial (n+1) terms of g.f.
 *          A097191(y)^(n+1), where R_n(1/3) = 9^n for all n>=0.
 * https://oeis.org/A097192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(n) = a(n-1) * 3*(9*n-1) / n (n ≥ 1) Recurrence:
 *            n*a(n) + 3*(1-9*n)*a(n-1) = 0 (All divisions are exact.)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = (seq_t)1;   /* a(0) = 1 */
    for (idx_t k = 1; k <= n; ++k)
    {
        /* numerator = 3*(9*k-1) */
        seq_t num = (seq_t)3 * ((seq_t)9 * k - (seq_t)1);

        if (sizeof(seq_t) <= 8) /* 64‑bit or smaller: use 128‑bit for safety */
        {
            unsigned __int128 tmp = (unsigned __int128)res *
                                    (unsigned __int128)num;
            res = (seq_t)(tmp / (unsigned __int128)k);
        }
        else /* arbitrary‑precision type: rely on its multiplication/division */
        {
            res = res * num;
            res = res / k;
        }
    }
    return res;
}
