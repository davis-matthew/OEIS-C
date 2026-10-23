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
 * Helpers:   big_pow, calc_phi, factorial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A047916: Triangular array read by rows
   a(n,k) = φ(n/k) * (n/k)^k * k!   if k | n else 0   (1 ≤ k ≤ n)
   The sequence a(n) is the flattened array: a(1), a(2), a(3), ...   */
#include "../utils/calc_phi.h"
#include "../utils/factorial.h"
#include "../utils/big_pow.h"
#include "../utils/floor_sqrt.h"

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
    seq_t phi_val = calc_phi((seq_t)q);
    seq_t pow_val  = big_pow((seq_t)q, (seq_t)k);
    seq_t fact_val = factorial((idx_t)k);

    return phi_val * pow_val * fact_val;
}
