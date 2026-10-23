/*
 * A175686  a(n) = binomial(n-j-1,j) + binomial(n-j,j-1) with j=
 *          floor((n-1)/2).
 * https://oeis.org/A175686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 147, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS sequence A175686
   Formula: 
     let k = n/2
     a(n) = k*(k*k + 5)/6     for even n
     a(n) = (n*n + 7)/8       for odd n
   (Equivalently: a(n) = (42 + 20*n + 6*n^2 + n^3 + (-1)^n*(-42 + 20*n - 6*n^2 + n^3))/96)
   This is an efficient O(1) implementation using only integer arithmetic.
*/

seq_t a(idx_t n)
{
    /* use 64‑bit intermediate to avoid overflow for the tested n range */
    if ((n & 1ULL) == 0)                 /* even n */
    {
        idx_t k = n >> 1;                /* k = n/2 */
        unsigned long long kk = (unsigned long long)k * k;   /* k^2   */
        unsigned long long tmp = kk + 5;                    /* k^2+5 */
        unsigned long long res = ((unsigned long long)k * tmp) / 6; /* k*(k^2+5)/6 */
        return (seq_t)res;
    }
    else                                 /* odd n */
    {
        unsigned long long nn = (unsigned long long)n * n;     /* n^2   */
        unsigned long long res = (nn + 7) / 8;                 /* (n^2+7)/8 */
        return (seq_t)res;
    }
}
