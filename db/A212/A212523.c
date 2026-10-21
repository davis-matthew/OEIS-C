/*
 * A212523  Number of (w,x,y,z) with all terms in {1,...,n} and w+x<y+z.
 * https://oeis.org/A212523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n*(3*(n-1)^2 + 7*(n-1) + 5)/6 for n ≥ 1, a(0) = 0.
 *            This equals the number of 4‑tuples (w,x,y,z) with
 *            w,x,y,z∈{1,…,n} and w+x<y+z
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 304, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;   /* a(0)=0, a(1)=0 */

    uint64_t m  = (uint64_t)(n - 1);           /* m = n-1 */
    uint64_t a1 = m;                          /* m */
    uint64_t a2 = m + 1;                      /* m+1 */
    uint64_t a3 = 3ULL * m * m + 7ULL * m + 5ULL; /* 3m^2 + 7m + 5 */
    uint64_t num = a1 * a2 * a3;              /* intermediate numerator */
    uint64_t res = num / 6ULL;                /* exact division by 6 */

    return (seq_t)res;
}
