/*
 * A059848  As a square table by antidiagonals, the n-digit number which in
 *          base k starts 1010101...
 * https://oeis.org/A059848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, base
 * Formula:   a(n) is the n‑th entry of the infinite table T(i,j) (i,j ≥ 0)
 *            read by anti‑diagonals. T(0,k)=0, T(1,k)=1,
 *            T(i,k)=T(i-2,k)+k^(i-1) (i≥2) Documented by MIT
 * Range:     u8 n <= 49, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/isqrt_u64.h"

/* Compute T(i,j) using the recurrence:
   even i  → sum of k^(odd)  (k=j)
   odd  i  → 1 + sum of k^(even)
*/
static seq_t T_of_i_j(idx_t i, idx_t j)
{
    if (i == 0) return 0;
    if (i == 1) return 1;

    seq_t sum = (i & 1) ? 1 : 0;   /* odd i: initial 1, even i: 0 */
    seq_t pow = 1;                 /* k^0 */
    for (idx_t exp = 1; exp <= i - 1; ++exp)
    {
        pow = pow * (seq_t)j;      /* k^exp */
        if ((i & 1) == 0)          /* even i → add odd exponents */
        {
            if (exp & 1) sum += pow;
        }
        else                        /* odd i  → add even exponents */
        {
            if ((exp & 1) == 0) sum += pow;
        }
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Find anti‑diagonal d and offset i:
       n = d*(d+1)/2 + i,   0 ≤ i ≤ d.
    */
    uint64_t dd = isqrt_u64(8ULL * (unsigned long long)n + 1ULL);
    idx_t d = (idx_t)((dd - 1ULL) / 2ULL);

    /* adjust for possible rounding error of floating sqrt */
    while ((unsigned __int128)d * (d + 1) / 2 > n) --d;
    while ((unsigned __int128)(d + 1) * (d + 2) / 2 <= n) ++d;

    uint64_t base = (unsigned __int128)d * (d + 1) / 2;
    idx_t i = n - base;          /* row index within the diagonal */
    idx_t j = d - i;             /* column index */

    return T_of_i_j(i, j);
}
