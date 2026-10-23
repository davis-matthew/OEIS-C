/*
 * A181176  The minimum absolute value obtainable by partitioning the first n
 *          consecutive integers into two sets 1,2,...,k and k+1,k+2,...,n,
 *          and subtracting the two sums of the sets.
 * https://oeis.org/A181176
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min(T_n - 2*T_k, 2*T_(k+1) - T_n) where T_m = m*(m+1)/2
 *            and k is the largest integer with k*(k+1) <= T_n
 * Range:     u8 n <= 20, u16 n <= 82, u32 n <= 82, u64 n <= 82, u128 n <= 82,
 *            big any n
 * Helpers:   triangular_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/triangular_calc.h"

 
/* 
   Finds the largest k such that k*(k+1) <= T_n.
   Binary search on the interval [0, n]. */ 
static inline idx_t max_k(idx_t n)
{
    seq_t Tn = triangular_calc(n);
    idx_t lo = 0, hi = n;          /* hi inclusive */
    while (lo < hi)
    {
        idx_t mid = lo + ((hi - lo + 1) >> 1);   /* upper mid */
        seq_t prod = (seq_t)mid * (mid + 1);
        if (prod <= Tn)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}
 
/*  External linkage as required. */
seq_t a(idx_t n)
{
    seq_t Tn = triangular_calc(n);
    idx_t k = max_k(n);
    seq_t Tk   = triangular_calc(k);
    seq_t Tk1  = triangular_calc(k + 1);
    seq_t diff1 = Tn - 2 * Tk;
    seq_t diff2 = 2 * Tk1 - Tn;
    return diff1 < diff2 ? diff1 : diff2;
}
