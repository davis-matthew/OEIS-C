/*
 * A159923  Rectangular array read by antidiagonals: a(m,n) = greatest
 *          noncomposite (1 or prime) that divides both m and n.
 * https://oeis.org/A159923
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = greatest non‑composite (1 or prime) divisor of gcd(i,j),
 *            where (i,j) are the antidiagonal coordinates of the n‑th term
 *            (1‑based). If gcd(i,j)=1 the answer is 1
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd, largest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/largest_prime_factor.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal level d such that
     * (d-1)*d/2 ≥ n  and (d-2)*(d-1)/2 < n */
    idx_t d = 2;
    while ((d - 1) * d / 2 < n)
        ++d;

    /* Number of terms before this diagonal */
    idx_t prev = (d - 2) * (d - 1) / 2;

    /* Zero‑based position inside the diagonal */
    idx_t pos = n - prev - 1;

    /* 1‑based coordinates (i,j) */
    idx_t i = pos + 1;
    idx_t j = d - i;

    /* GCD of the coordinates */
    seq_t g = gcd((seq_t)i, (seq_t)j);

    if (g == (seq_t)1)
        return (seq_t)1;

    /* largest_prime_factor expects a 64‑bit integer */
    return (seq_t)largest_prime_factor((uint64_t)g);
}
