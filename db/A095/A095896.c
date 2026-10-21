/*
 * A095896  Triangle read by rows: T(n,k) = n^(n-k+1), n>=1, 1<=k<=n.
 * https://oeis.org/A095896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 21, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A095896 : triangle read by rows, formula
 *     T(r,k) = r^(r-k+1),   1 ≤ k ≤ r,
 * a(n) is the value at linear index n (offset = 1). */
#include <stdint.h>   /* for __int128 */

/* compute r,k for a given linear index n, then raise r to the power
 * (r-k+1).  All arithmetic on indices uses idx_t; all arithmetic on
 * values uses seq_t.  Intermediate triangular calculations are done
 * in __int128 to avoid overflow. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; undefined for n==0 */
        return (seq_t)0;

    /* Find smallest r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (( (__int128)r * (r + 1) / 2 ) < n)
        ++r;

    /* Number of elements before the current row */
    idx_t before = ( (__int128)(r - 1) * r ) / 2;
    idx_t k = n - before;                 /* 1 ≤ k ≤ r        */
    idx_t exp = r - k + 1;                /* exponent (>=1)    */

    /* Compute r^exp using repeated multiplication */
    seq_t base = (seq_t)r;
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;

    return result;
}
