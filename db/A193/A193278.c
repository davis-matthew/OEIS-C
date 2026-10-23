/*
 * A193278  Triangle T(n, k) where T(n, k) = T(k, n), T(n, n*k) = k, and T(n,
 *          n+k) = T(n, k) if k not divisible by n.
 * https://oeis.org/A193278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) where r is the smallest integer with r(r+1)/2 ≥ n
 *            and k = n - r(r-1)/2. The triangle T is defined by T(a,b) =
 *            T(b,a) T(a,a) = 1 if b % a == 0 → T(a,b) = b/a otherwise →
 *            T(a,b) = T(a, b-a) (b > a) This implements the OEIS definition
 *            of A193278
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   T_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/T_calc.h"

seq_t a(idx_t n)
{
    /* Index 0 is outside the OEIS offset; return 0 for safety. */
    if (n == 0)
        return 0;

    /* Find the row r so that r(r-1)/2 < n ≤ r(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Compute the column k (1‑based).  k ≤ r for all indices. */
    idx_t prev = (r - 1) * r / 2;   /* elements before row r */
    idx_t k = n - prev;             /* column index */

    return T_calc(r, k);
}
