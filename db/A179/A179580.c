/*
 * A179580  Number of permutations of {1, ..., n+5} with deficiencies #{i :
 *          p(i) < i} exceeding the exceedances #{i : p(i) > i} by n or more.
 * https://oeis.org/A179580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)*(n^2-2*n-36)/6 + (n+7)*2^(n+4)
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 23, u64 n <= 54, u128 n <= 100,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* use 128‑bit signed arithmetic for the first term, which can
       be negative for small n */
    __int128 tn = (__int128)n;
    __int128 t   = tn*tn - (__int128)2*tn - (__int128)36;
    __int128 term1 = (tn + (__int128)5) * t / (__int128)6;

    /* second term uses the provided pow2 routine */
    seq_t term2 = (seq_t)(tn + (__int128)7) * pow2(n + (idx_t)4);

    if (term1 >= 0)
        return term2 + (seq_t)term1;
    else
        return term2 - (seq_t)(-term1);
}
