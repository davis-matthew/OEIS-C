/*
 * A239631  Triangular array read by rows: T(n,k) is the number of parts equal
 *          to k over all palindromic compositions of n, n>=1, 1<=k<=n.
 * https://oeis.org/A239631
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) by flattening the triangular array defined by
   the explicit formulas for T(n,k).  The first n terms are in
   the OEIS entry A239631.  The function is fully self‑contained
   and does not rely on any external helper functions.  */

#include <stdint.h>

/* shift left by exp, safe for all seq_t sizes (int promotion)
   exp is guaranteed to be < 64 in the usage here. */
static inline seq_t pow2(uint64_t exp)
{
    return (seq_t)1 << exp;
}

/* a(n) – nth term of OEIS A239631,  n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined; safety return */

    /* Find the row r such that r*(r+1)/2 >= n.
       Since n <= 10000 (typical), a simple linear search is
       fast enough (max r ~ 140). */
    idx_t r = 1;
    while ((r * (r + 1) / 2) < n) r++;

    /* Index of the first element of this row (1‑based) */
    idx_t prev = (r - 1) * r / 2;

    /* Column index k in [1..r] */
    idx_t k = n - prev;

    /* Explicit formulas from OEIS paragraph:
       see formula section for T(r,k).  */

    if (r == k) return 1;             /* T(n,n) = 1 */

    /* Case 0 < k < r < 2k, parity differs -> 0 */
    if (k < r && r < 2 * k)
    {
        if ((r & 1) != (k & 1)) return 0;

        /* Same parity: T(n,k) = 2^((n-k)/2 -1) */
        idx_t exp = (r - k) / 2 - 1;   /* r-k is even and >=2 */
        return pow2(exp);
    }

    /* Case r >= 2k */
    {
        idx_t half = r >> 1;           /* floor(r/2) */
        idx_t exp  = half - k;         /* >= 0 */
        seq_t base = pow2(exp);

        /* Main term */
        seq_t term = 2 + (seq_t)(half - k);

        /* If parity matches, add extra contribution 2^floor((k+1)/2-1) */
        if ((r & 1) == (k & 1))
        {
            uint64_t add_exp = ( (k + 1) / 2 ) - 1;  /* floor((k+1)/2)-1 */
            if (add_exp > 0) term += pow2(add_exp);
            else term += 1;          /* pow2(0) == 1 */
        }

        return base * term;
    }
}
