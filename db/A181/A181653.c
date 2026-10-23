/*
 * A181653  Generalized (conditional) Riordan array with k-th column generated
 *          by x^k*(1+x) if k mod 2 = 0, x^k*(1+x+x^2) otherwise.
 * https://oeis.org/A181653
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: A181653 is the row‑wise enumeration of a binary triangle.
 * For column k (0‑based): 
 *   g_k(x) = x^k*(1+x)          if k is even,
 *   g_k(x) = x^k*(1+x+x^2)      if k is odd.
 * Hence T(n,k)=1 iff 
 *   k==n, or k+1==n, 
 *   or (k odd and k+2==n). 
 * The sequence a(n) lists T(r,c) with (r,c) the row and column of
 * the n‑th entry in row‑wise order (row 0 has 1 element, row r has r+1).
 */

#include <stdbool.h>

static inline idx_t triangular(idx_t r)
{
    return (idx_t)(((__uint128_t)r * (r + 1)) >> 1);
}

seq_t a(idx_t n)
{
    /* Find the row r such that triangular(r) <= n < triangular(r+1). */
    idx_t r = 0;
    while (triangular(r + 1) <= n)
        ++r;

    /* Column index c within that row. */
    idx_t c = n - triangular(r);

    /* Determine whether the entry is 1 or 0. */
    bool val = (r == c) || (r == c + 1);
    if (!val && (c & 1))   /* odd column */
        val = (r == c + 2);

    return (seq_t)val;
}
