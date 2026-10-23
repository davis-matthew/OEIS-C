/*
 * A047917  Triangular array read by rows: a(n,k) = phi(n/k)*(n/k)^k*k!/n if
 *          k|n else 0 (1<=k<=n).
 * https://oeis.org/A047917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy
 * Formula:   Triangular array a(n,k) = φ(n/k)*(n/k)^k*k!/n if k|n else 0;
 *            a(n) is the n‑th element read row‑wise
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 104, u64 n <= 252,
 *            u128 n <= 665, big any n
 * Helpers:   big_pow, calc_phi, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/big_pow.h"
#include "../utils/factorial.h"
#include "../utils/calc_phi.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;            /* safety check – offset is 1 */

    /* Determine the row r such that T_{r-1} < n <= T_r, where
       T_r = r*(r+1)/2.  Use double sqrt for simplicity (n <= ~1e6).   */
    double d = sqrt((double)n * 8.0 + 1.0);
    idx_t r = (idx_t)((d - 1.0) / 2.0);          /* floor estimate   */
    /* Correct the estimate if necessary */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Previous triangular number */
    unsigned __int128 t_prev = (unsigned __int128)r * (r - 1) / 2;
    idx_t k = n - (idx_t)t_prev;   /* column index within the row */

    /* If k does not divide r, the value is 0. */
    if (r % k != 0) return 0;

    idx_t m = r / k;                     /* n/k in the formula      */

    seq_t phi_m   = calc_phi((seq_t)m);   /* φ(m)                    */
    seq_t pow_mk  = big_pow((seq_t)m, k); /* (m)^k                    */
    seq_t fact_k  = factorial(k);        /* k!                       */

    /* Numerator = φ(m) * (m)^k * k!   */
    seq_t num = phi_m;
    num = num * pow_mk;
    num = num * fact_k;

    /* Divide by r (exact division). */
    return num / (seq_t)r;
}
