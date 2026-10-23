/*
 * A134866  Table read by antidiagonals: T(n,k) = sigma(gcd(n,k)).
 * https://oeis.org/A134866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9895, u16 n <= 9895, u32 n <= 9895, u64 n <= 9895,
 *            u128 n <= 9895, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A134866:  T(n,k) = sigma(gcd(n,k)) read by antidiagonals.
   a(n) = sigma(gcd(i,j)) where (i,j) are the row and column indices
   of the n-th element in the antidiagonal ordering:
   - Diagonals are indexed by s = i + j (s ≥ 2).
   - The cumulative count up to diagonal s is C(s) = s*(s-1)/2.
   - For a given n, find the smallest s with C(s) ≥ n.
   - Let offset = n - C(s-1); then i = offset, j = s - offset.
*/

#include "../utils/gcd.h"

/* Compute σ(m) = sum of positive divisors of m (m > 0). */
static seq_t sigma_of(seq_t m)
{
    /* For m == 0 (shouldn't happen in this sequence) return 0. */
    if (m == 0)
        return 0;

    seq_t sum = 0;
    seq_t i = 1;
    /* Loop up to √m; i*i will never overflow seq_t because i ≤ √m. */
    while (i * i <= m) {
        if (m % i == 0) {
            seq_t j = m / i;
            sum += i;
            if (j != i)
                sum += j;
        }
        ++i;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                 /* undefined for 0, harmless */

    idx_t s = 2;                   /* current diagonal (sum i+j) */
    idx_t t = 1;                   /* cumulative count C(s) */

    /* Find diagonal s where t ≥ n.  t = C(s) = s*(s-1)/2. */
    while (t < n) {
        ++s;
        t += s - 1;                /* add size of new diagonal (s-1) */
    }

    idx_t prev_t = t - (s - 1);    /* C(s-1) = (s-2)*(s-1)/2 */
    idx_t offset = n - prev_t;     /* 1‑based position inside diagonal */

    idx_t i = offset;
    idx_t j = s - offset;

    seq_t g = gcd((seq_t)i, (seq_t)j);      /* gcd of row and column */
    return sigma_of(g);                    /* sum of divisors of gcd */
}
