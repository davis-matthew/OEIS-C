/*
 * A048084  Number of nonempty subsets of {1,2,...,n} in which exactly 2/3 of
 *          the elements are <= (n+3)/3.
 * https://oeis.org/A048084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{h≥1}^{min(⌊L/2⌋,H)} binom(L,2h) * binom(H,h), where L
 *            = ⌊(n+3)/3⌋ and H = n − L. This counts non‑empty subsets of
 *            {1,…,n} whose two‑thirds of elements lie in the low part {1,…,L}
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    idx_t L = (n + 3) / 3;       /* floor((n+3)/3) */
    idx_t H = n - L;             /* elements > (n+3)/3 */

    seq_t result = 0;

    idx_t hmax = L / 2;                 /* largest h with 2h ≤ L */
    if (hmax > H)
        hmax = H;                       /* also must fit in high part */

    for (idx_t h = 1; h <= hmax; ++h) {
        idx_t low = 2 * h;              /* 2h ≤ L by construction   */
        seq_t lowCnt  = binom(L, low);  /* C(L, 2h)                    */
        seq_t highCnt = binom(H, h);    /* C(H,  h)                    */
        result += lowCnt * highCnt;     /* term contribution            */
    }

    return result;   /* 0 for n=1,2 as the sum is empty */
}
