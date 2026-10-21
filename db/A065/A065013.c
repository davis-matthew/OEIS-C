/*
 * A065013  Integers for which the periodic part of the continued fraction for
 *          the square root of n begins with 10.
 * https://oeis.org/A065013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A065013: integers n for which the periodic part of the continued fraction of sqrt(n) begins with 10.
   The algorithm enumerates n in increasing order by iterating the integral part a0 = ⌊√n⌋.
   For a given a0 let k = n − a0².  The first periodic term equals ⌊2a0/k⌋, so we need
   10 ≤ 2a0/k < 11, i.e.  ⌊2a0/11⌋+1 ≤ k ≤ ⌊a0/5⌋.  All such (a0,k) pairs generate a valid n.
   The nth element is found by counting up to n without storing all terms. */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    idx_t cnt = 0;                /* number of terms found so far */

    for (idx_t a0 = 1; ; ++a0)    /* iterate the integer part of √n */
    {
        seq_t a0sq = (seq_t)a0 * (seq_t)a0;   /* a0² */

        /* k must satisfy 10 ≤ 2⋅a0/k < 11   →   ⌊2a0/11⌋+1 ≤ k ≤ ⌊a0/5⌋ */
        idx_t k_min = ((idx_t)2 * a0) / 11 + 1;
        idx_t k_max = a0 / 5;

        if (k_min <= k_max) {
            for (idx_t k = k_min; k <= k_max; ++k) {
                ++cnt;
                if (cnt == n)
                    return a0sq + (seq_t)k;
            }
        }
    }
}
