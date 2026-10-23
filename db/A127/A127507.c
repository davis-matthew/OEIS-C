/*
 * A127507  Triangle read by rows: T(n,k) = mu(n) where 1<=k<=n and
 *          mu=A008683.
 * https://oeis.org/A127507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, easy, sign
 * Formula:   not stated
 * Range:     i8 n <= 55, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A127507 – triangle of Möbius values flattened row‑by‑row.
   a(n) = μ(r) where r is the unique integer with
          r*(r+1)/2 ≥ n > (r‑1)*r/2.  */

/* The Möbius function helper is provided in utils. */
#include <stdint.h>
#include "../utils/mobius.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by specification; for n = 0 the result is undefined. */

    /* Binary search for the row number r. */
    idx_t low  = 1;
    idx_t high = n;            /* r cannot exceed n */
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __uint128_t tri = (__uint128_t)mid * (mid + 1) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;             /* row containing the n‑th entry */
    return mobius(r);          /* μ(r) as seq_t */
}
