/*
 * A130540  Triangle read by rows T(n,k) in which column k lists the terms of
 *          A000203 interspersed with (k-1) zeros, 1 <= k <= n.
 * https://oeis.org/A130540
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the triangle T(n,k) read by rows, * where T(n,k) =
 *            sigma(n/k) if k divides n, otherwise 0. * Here sigma(m) is the
 *            sum of all positive divisors of m. * The linear index n is
 *            transformed to row r and column k by * r = smallest integer with
 *            r(r+1)/2 ≥ n, * k = n – (r−1)r/2. Then a(n) = sigma...
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute σ(m), the sum of divisors of m.  m ≥ 1. */
static inline seq_t sigma_simple(idx_t m)
{
    if (m == 1)
        return (seq_t)1;
    seq_t sum = (seq_t)1 + (seq_t)m;          /* include 1 and m */
    for (idx_t d = 2; d <= m / d; ++d) {      /* avoid overflow in d*d */
        if (m % d == 0) {
            if (d == m / d)          /* perfect square divisor */
                sum += (seq_t)d;
            else
                sum += (seq_t)d + (seq_t)(m / d);
        }
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* offset guard – not used */

    /* Find the row r such that (r-1)r/2 < n ≤ r(r+1)/2. */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        __int128 tri = (__int128)mid * (mid + 1) / 2;
        if (tri >= ( __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Column index k (1‑based) in this row. */
    idx_t prev_tri = (r - 1) * r / 2;
    idx_t k = n - prev_tri;          /* 1 ≤ k ≤ r */

    if (r % k != 0)
        return (seq_t)0;

    idx_t m = r / k;
    return sigma_simple(m);
}
