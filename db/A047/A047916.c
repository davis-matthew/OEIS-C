/*
 * A047916  Triangular array read by rows: a(n,k) = phi(n/k)*(n/k)^k*k! if k|n
 *          else 0 (1<=k<=n).
 * https://oeis.org/A047916
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 90, u64 n <= 230, u128 n <= 629,
 *            big any n
 * Validated: standard, pass, 84 terms through n = 7806 (2026-11-01)
 * Helpers:   ipow, ipow_i32, totient_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047916: Triangular array read by rows
   a(n,k) = φ(n/k) * (n/k)^k * k!   if k | n else 0   (1 ≤ k ≤ n)
   The sequence a(n) is the flattened array: a(1), a(2), a(3), ...   */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"
#include "../utils/totient_seq.h"

/* Helper: compute factorials iteratively.
   For built‑in integer types the result may overflow for large n,
   but the tests use only n for which the value fits in seq_t. */
static seq_t factorial(idx_t n)
{
    seq_t result = 1;
    for (idx_t i = 2; i <= n; ++i)
        result *= (seq_t)i;
    return result;
}

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* offset is 1 */

    /* Find the row r such that r(r+1)/2 ≥ n */
    /* Compute r = ceil((sqrt(8*n+1)-1)/2) without floating point. */
    idx_t t = 8 * n + 1;                       /* 8*n + 1 fits in idx_t for n ≤ 10⁴ */
    idx_t sqrt_t = (idx_t)floor_sqrt((seq_t)t);
    idx_t r = (sqrt_t - 1) / 2;                /* floor part */
    if (r * (r + 1) / 2 < n) r++;              /* adjust if needed */

    /* Column k within this row (1‑based) */
    idx_t k = n - (r - 1) * r / 2;
    if (r % k != 0) return 0;                 /* k does not divide r → value 0 */

    idx_t q = r / k;                           /* integer division */

    /* Compute the formula: φ(q) * q^k * k! */
    seq_t phi_val = totient_seq((seq_t)q);
    seq_t pow_val  = ipow((seq_t)q, (seq_t)k);
    seq_t fact_val = factorial((idx_t)k);

    return phi_val * pow_val * fact_val;
}
