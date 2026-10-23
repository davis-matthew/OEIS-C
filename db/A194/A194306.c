/*
 * A194306  Triangular array: g(n,k) = number of fractional parts (i*Pi) in
 *          interval [(k-1)/n, k/n], for 1 <= i <= 2n, 1 <= k <= n.
 * https://oeis.org/A194306
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = g(r,k) where r = smallest integer with r(r+1)/2 >= n, k =
 *            n - r(r-1)/2. g(r,k) = number of i (1 <= i <= 2r) such that
 *            fractional part of i*Pi lies in the interval [(k-1)/r, k/r].
 *            Computed by brute‑force check
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

static const long double PI = acosl(-1.0L);

seq_t a(idx_t N)
{
    /* determine the row r and column k for index N */
    idx_t r;
    /* initial approximation using double sqrt */
    double t = sqrt(1.0 + 8.0 * (double)N);
    r = (idx_t)((t - 1.0) * 0.5);
    while (r * (r + 1) / 2 < N) r++;
    while ((r >= 1) && (r - 1) * r / 2 >= N) r--;

    idx_t T_prev = (r - 1) * r / 2;          /* triangular number before row r */
    idx_t k = N - T_prev;                    /* 1‑based column index */

    /* pre‑compute interval bounds */
    long double left  = (long double)(k - 1) / (long double)r;
    long double right = (long double)k      / (long double)r;
    const long double eps = 1e-18L;          /* safety margin */

    seq_t cnt = 0;
    for (idx_t i = 1; i <= 2 * r; ++i)
    {
        long double f = fmodl((long double)i * PI, 1.0L);
        if (f < 0.0L) f += 1.0L;            /* guard against negative residue */
        if (f + eps >= left && f < right)
            cnt++;
    }
    return cnt;
}
