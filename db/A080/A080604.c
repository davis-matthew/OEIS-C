/*
 * A080604  Triangular array of hypotenuses of right triangles with integer
 *          legs: T(n,k) = round(sqrt(n^2 + k^2)), 1 <= k <= n.
 * https://oeis.org/A080604
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = round( sqrt( r² + k² ) ), where r = smallest integer with
 *            r(r+1)/2 ≥ n, and k = n – r(r−1)/2 (so 1 ≤ k ≤ r)
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* ----- find the row r ----- */
    double dn = (double)n;
    unsigned long long r_est = (unsigned long long)((sqrt(8.0*dn + 1.0) - 1.0) * 0.5);
    idx_t r = r_est;
    while ((unsigned __int128)r * (r + 1ULL) / 2ULL < n)
        ++r;

    /* ----- column k (1‑based) ----- */
    idx_t prev = (unsigned __int128)(r - 1ULL) * r / 2ULL;
    idx_t k   = n - prev;   /* 1 ≤ k ≤ r */

    /* ----- compute the rounded hypotenuse ----- */
    unsigned long long rr = (unsigned long long)r;
    unsigned long long kk = (unsigned long long)k;
    unsigned long long sum = rr * rr + kk * kk;            /* r² + k² <= 2*n² */

    /* floor sqrt for the 64‑bit sum */
    unsigned long long s  = (unsigned long long)floor(sqrt((long double)sum));
    unsigned long long s1 = s + 1;

    unsigned long long diff1 = sum - s  * s;   /* distance to lower square */
    unsigned long long diff2 = s1 * s1 - sum;   /* distance to upper square */

    /* ties round up (i.e. to the larger integer) */
    unsigned long long res = (diff1 >= diff2) ? s1 : s;
    return (seq_t)res;
}
