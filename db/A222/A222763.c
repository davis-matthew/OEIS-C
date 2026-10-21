/*
 * A222763  Number of n X 2 0..1 arrays with exactly floor(nX2/2) elements
 *          unequal to at least one horizontal or antidiagonal neighbor, with
 *          new values introduced in row major 0..1 order.
 * https://oeis.org/A222763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{odd k=3}^{kmax} C( (2n+1 + k)/2 - 1 , (k+1)/2 - 1 )
 *            * C( (2n+1 - k)/2 - 1 , (k-1)/2 - 1 ) where kmax = n+1 if n is
 *            even, otherwise n (kmax is always odd). The sum counts integer
 *            compositions of 2n+1 whose length equals their
 *            reverse‑alternating sum
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"   /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    /* Handle the trivial initial values directly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Determine the largest odd k that may contribute. */
    idx_t kmax = (n % 2 == 0) ? (n + 1) : n;   /* always odd */

    /* m = 2n + 1 (odd). All divisions by 2 are exact. */
    seq_t m = (seq_t)2 * n + 1;

    seq_t result = 0;

    for (idx_t k = 3; k <= kmax; k += 2) {
        /* Odd k => k = 2e + 1 with e = (k-1)/2, o = e+1 = (k+1)/2. */
        idx_t o = (k + 1) / 2;   /* number of odd‑position terms */
        idx_t e = (k - 1) / 2;   /* number of even‑position terms */

        /* A = (m + k)/2, B = (m - k)/2  (both integers). */
        seq_t A = (m + k) / 2;
        seq_t B = (m - k) / 2;

        /* Compute the two binomial coefficients. */
        seq_t c1 = binomial((idx_t)(A - 1), (idx_t)(o - 1));
        seq_t c2 = binomial((idx_t)(B - 1), (idx_t)(e - 1));

        result += c1 * c2;
    }

    return result;
}
