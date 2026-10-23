/*
 * A138770  Triangle read by rows: T(n,k) is the number of permutations of
 *          {1,2,...,n} such that there are exactly k entries between the
 *          entries 1 and 2 (n>=2, 0<=k<=n-2).
 * https://oeis.org/A138770
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 29, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A138770: Triangle read by rows.  The sequence a(n) for n≥2 is the
 * flattened triangle of values T(m,k) = 2*(m-k-1)*(m-2)!, where the rows
 * begin at m=2 with one element, then m=3 with two elements, etc.
 * For a given index n (offset 2, so n=2 is the first element) we find
 * the unique row m such that
 *
 *   (m-1)(m-2)/2 ≤ n-2 < m(m-1)/2
 *
 * and the position within that row is
 *   k = (n-2) - (m-1)(m-2)/2.
 *  Then a(n) = 2*(m-k-1)*(m-2)!.
 *
 *  The function uses the provided factorial routine.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    if (n < 2)  /* behaviour below the offset is unspecified */
        return 0;

    /* Convert to 0‑based offset into the triangle */
    idx_t t = n - 2;

    /* Find the row m.  We need the smallest m with m(m-1)/2 > t. */
    idx_t m = 2;
    while ((m * (m - 1)) / 2 <= t)
        ++m;

    /* Position within the row (0‑based). */
    idx_t prev = (m - 1) * (m - 2) / 2;
    idx_t k = t - prev;

    /* Compute T(m,k) = 2*(m-k-1)*(m-2)! */
    seq_t fact = factorial(m - 2);
    seq_t multiplier = (seq_t)(2 * (m - k - 1));
    return fact * multiplier;
}
