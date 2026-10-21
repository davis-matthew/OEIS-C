/*
 * A084885  Triangular array, read by rows: T(n,k) = denominator of arithmetic
 *          derivative of n/k, 1<=k<=n.
 * https://oeis.org/A084885
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = denominator of the reduced fraction * (k*A003415(r) –
 *            r*A003415(k)) / k² * where (r,k) is the row and column that
 *            contain the n‑th term * (rows are 1‐based, columns run from 1 to
 *            r)
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   adderivative, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/adderivative.h"
#include "../utils/gcd.h"

/* Integer square root (floor) for 64‑bit values. */
static idx_t isqrt(idx_t x)
{
    idx_t lo = 0, hi = x;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        if (mid <= x / mid) lo = mid + 1;
        else hi = mid - 1;
    }
    return hi;
}

/* Helper to convert a linear index n into row r and column k. */
static void index_to_rc(idx_t n, idx_t *r, idx_t *k)
{
    /* Find the smallest r such that n ≤ r(r+1)/2. */
    idx_t s = 8 * n + 1;
    idx_t sqrt_s = isqrt(s);
    *r = (sqrt_s - 1) >> 1;                     /* floor((sqrt(8n+1)-1)/2) */
    /* Adjust if under‑estimating. */
    while ((*r * (*r + 1)) / 2 < n) ++(*r);
    *k = n - (*r * (*r - 1)) / 2;                /* 1‑based column */
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined for n=0 */

    idx_t r, k;
    index_to_rc(n, &r, &k);

    seq_t dr = adderivative(r);
    seq_t dk = adderivative(k);

    seq_t prod_r = r * dk;                      /* r * A₃₄₁₅(k)  */
    seq_t prod_k = k * dr;                      /* k * A₃₄₁₅(r)  */

    /* Difference of two unsigned products (avoids negative underflow). */
    seq_t numer = (prod_k >= prod_r) ? (prod_k - prod_r) : (prod_r - prod_k);

    seq_t denom_raw = k * k;                    /* k² */

    seq_t g = gcd(numer, denom_raw);
    return denom_raw / g;
}
