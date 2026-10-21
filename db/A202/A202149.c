/*
 * A202149  Triangle read by rows: T(n, k) = mod(2^k, n), where 1 <= k < n.
 * https://oeis.org/A202149
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n‑th term of the triangle read by rows, * defined by
 *            T(r,k) = 2^k mod r with 1 ≤ k < r, * where row r starts at index
 *            n=2. The sequence is * flattened row‑wise: a(2)=0, a(3)=2,
 *            a(4)=1, a(5)=2, …
 * Range:     u8 n <= 4951, u16 n <= 4951, u32 n <= 4951, u64 n <= 4951,
 *            u128 n <= 4951, big any n
 * Helpers:   mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mod_pow.h"   /* seq_t mod_pow(seq_t base, idx_t exp, seq_t mod); */

/* Find smallest r ≥ 2 such that r*(r-1)/2 ≥ m (m = n-1).  */
static inline idx_t row_for(idx_t m)
{
    /* Rough estimate using double; the loops below correct any error. */
    double approx = sqrt(2.0 * (double)m) + 2.0;
    idx_t r = (idx_t)approx;
    if (r < 2) r = 2;

    /* Ascend until the inequality holds. */
    while ( ((__int128)r * (r - 1)) / 2 < (unsigned __int128)m ) {
        r++;
    }
    /* Descend if we overshot. */
    while (r > 2 && ((__int128)(r - 1) * (r - 2)) / 2 >= (unsigned __int128)m) {
        r--;
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* undefined below the offset */

    idx_t m = n - 1;              /* zero–based index in the flattened triangle */
    idx_t r = row_for(m);         /* row number (≥2) */

    /* Previous number of terms before row r. */
    idx_t prev = (r > 2) ? ((r - 2) * (r - 1)) / 2 : 0;

    idx_t k = m - prev;          /* 1‑based position within the row (1 ≤ k < r) */

    return mod_pow(2, k, (seq_t)r);   /* T(r,k) = 2^k mod r */
}
