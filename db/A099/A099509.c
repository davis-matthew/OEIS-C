/*
 * A099509  Triangle, read by rows, of trinomial coefficients arranged so that
 *          there are n+1 terms in row n by setting T(n,k) equal to the
 *          coefficient of z^k in (1 + z + z^2)^(n-[k/2]), for n>=k>=0, where
 *          [k/2] is the integer floor of k/2.
 * https://oeis.org/A099509
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n)th entry of the triangle of trinomial
 *            coefficients, read row by row. For row r (0‑based) and position
 *            k (0‑based), T(r,k) = coefficient of z^k in (1+z+z^2)^(r−⌊k/2⌋).
 *            Implementation: recover r and k for index n, then compute T(r,k)
 *            by summing over the possible number of z^2 terms j...
 * Range:     u8 n <= 49, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/choose.h"

/* ---------- integer square root for 64‑bit values ---------- */
static idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 t = (unsigned __int128)mid * mid;
        if (t <= x) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) >> 1;
    idx_t first_in_row = r * (r + 1) / 2;
    idx_t k = n - first_in_row;          /* position in row */
    idx_t floor_k2 = k >> 1;              /* floor(k/2) */
    idx_t exp = r - floor_k2;             /* exponent in (1+z+z^2)^exp */

    /* Sum over j */
    idx_t jmin = (k > exp) ? (k - exp) : 0;
    idx_t jmax = floor_k2;
    seq_t sum = 0;

    for (idx_t j = jmin; j <= jmax; ++j) {
        idx_t i = k - (j << 1);              /* i = k – 2j */
        seq_t term = choose(exp, j);
        term = term * choose(exp - j, i);
        sum += term;
    }
    return sum;
}
