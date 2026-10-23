/*
 * A122415  Triangle T(n,k) for 1 < k < n read by rows, where T(n,k) = 1 if
 *          gcd(n,k) is prime, 0 otherwise.
 * https://oeis.org/A122415
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 107, u16 n <= 107, u32 n <= 107, u64 n <= 107,
 *            u128 n <= 107, big any n
 * Helpers:   gcd, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A122415 – Triangle T(n,k) with gcd(n,k) prime */
#include <stdbool.h>
#include "../utils/gcd.h"
#include "../utils/is_prime.h"

/* a(n) = 1 if gcd(r,k) is prime, 0 otherwise,
   where r and k are determined by unfolding the triangle
   T(n,k) for 1 < k < n read by rows.  Offset = 3. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;          /* undefined below offset, safe fallback */

    /* zero‑based position in the flattened list */
    idx_t pos = n - 3;

    /* Find row r (starting from r = 3) using triangular numbers. */
    idx_t t   = 1;      /* t = r - 2, starts at 1 for row 3 */
    idx_t cum = 0;      /* cumulative count of elements up to previous row */

    while (cum + t <= pos) {   /* skip rows whose elements are before pos */
        cum += t;
        t   += 1;
    }

    idx_t r = t + 2;                      /* actual row number */
    idx_t k = (pos - cum) + 2;            /* column within that row */

    /* Compute gcd and test primality. */
    seq_t g = gcd((seq_t)r, (seq_t)k);
    return (seq_t)(is_prime((idx_t)g) ? 1 : 0);
}
