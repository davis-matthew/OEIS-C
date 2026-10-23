/*
 * A140648  Triangle T(n,m) which can create A140642 without help of
 *          Jacobsthal numbers.
 * https://oeis.org/A140648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, uned
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Derived from the triangle T(n,m).  For n ≥ 0 let r be the unique integer
   with r(r+1)/2 ≤ n < (r+1)(r+2)/2 (the row index) and let c = n – r(r+1)/2
   (the column in that row, 0‑based).  Then
       a(n) = T(r,c)
   where
       T(0,0)   = 1,
       T(1,0)   = 2,   T(1,1)   = 0,
       T(r,0)   = 2^r                      (r ≥ 2),
       T(r,1)   = 2^(r-2)                  (r ≥ 2),
       T(r,c)   = T(r-1, r-c+1)  for r ≥ 2, c ≥ 2.                                   
   These rules generate the triangle whose flattened row‑wise list is OEIS
   sequence A140648. */

#include <stdlib.h>

/* Compute 2^exp as a seq_t value.  Uses a loop to avoid undefined
   right‑shift of widths ≥ sizeof(seq_t)*8. */
static inline seq_t power_of_two(idx_t exp)
{
    seq_t val = 1;
    for (idx_t i = 0; i < exp; ++i)
        val <<= 1;
    return val;
}

/* Recursive definition of the triangle entry T(r,c).  All arguments are
   unsigned integer types large enough for the requested indices. */
static inline seq_t T(idx_t r, idx_t c)
{
    if (r == 0)
        return (seq_t)1;
    if (r == 1)
        return (c == 0) ? (seq_t)2 : (seq_t)0;
    if (c == 0)
        return power_of_two(r);
    if (c == 1)
        return power_of_two(r - 2);
    /* For c >= 2, use the recursive rule T(r,c) = T(r-1, r-c+1). */
    return T(r - 1, r - c + 1);
}

/* Public function requested by the build. */
seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    return T(r, c);
}
