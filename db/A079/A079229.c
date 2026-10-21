/*
 * A079229  Least k>0 such that rad(n+k) > rad(n), where rad is the squarefree
 *          kernel (A007947).
 * https://oeis.org/A079229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k>0 such that rad(n+k) > rad(n)
 * Range:     u8 n <= 662, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    seq_t rad_n = rad(n);          /* square‑free kernel of n */

    for (idx_t k = 1; ; ++k)       /* search for the smallest increase in rad */
    {
        idx_t m = n + k;
        seq_t rad_m = rad(m);
        if (rad_m > rad_n)         /* found the next greater rad */
            return (seq_t)k;      /* k fits in seq_t for the tested n */
    }
}
