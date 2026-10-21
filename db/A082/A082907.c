/*
 * A082907  A modified Pascal's triangle, read by rows, and modified as
 *          follows: binomial(n,j) is replaced by gcd(2^n, binomial(n,j)),
 *          i.e., the largest power of 2 dividing binomial(n,j).
 * https://oeis.org/A082907
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^min(v2( binomial(row,column) ), row) where (row,column)
 *            are the coordinates of n in the row‑major enumeration of
 *            Pascal's triangle (row 0 has 1 element, row 1 has 2, …)
 * Range:     u8 n <= 1274, u16 n <= 1274, u32 n <= 1274, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


static inline seq_t pow2(idx_t e)
{
    /* Return 2^e as seq_t; if it cannot be represented we return 0. */
    if (e >= (idx_t)(8 * sizeof(seq_t)))
        return (seq_t)0;
    seq_t r = 1;
    while (e--)
        r <<= 1;
    return r;
}

seq_t a(idx_t n)
{
    /* Find the row r and column j for index n. */
    idx_t r = 0;
    idx_t remaining = n;
    while (remaining >= (r + 1))
    {
        remaining -= (r + 1);
        r++;
    }
    idx_t j = remaining;  /* column (0‑based) */

    /* Exponent of 2 in binomial(r,j) using Kummer's theorem:
       v2 = popcount(j) + popcount(r-j) – popcount(r). */
    unsigned pc_r   = __builtin_popcountll((unsigned long long)r);
    unsigned pc_j   = __builtin_popcountll((unsigned long long)j);
    unsigned pc_rj  = __builtin_popcountll((unsigned long long)(r - j));
    unsigned v2     = pc_j + pc_rj - pc_r;

    idx_t exp = (r < (idx_t)v2) ? r : (idx_t)v2;
    return pow2(exp);
}
