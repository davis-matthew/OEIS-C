/*
 * A137520  Triangle read by rows: T(n, k) = (n!/3) * [t^n*x^k]
 *          (1-(4/(t^4-1))^2) * exp(t*x).
 * https://oeis.org/A137520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  less, tabl, sign
 * Formula:   a(n) = ((r-1)! / (3 * (k-1)!)) * a_{r-k}, where r is the row
 *            such that r(r+1)/2 ≥ n > r(r-1)/2, k = n - r(r-1)/2, a_t = -15
 *            if t=0, a_t = -16*(s+1) if t = 4s > 0, a_t = 0 otherwise
 * Range:     i8 n <= 10, i16 n <= 36, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assumes the existence of the helper function
   seq_t factorial(idx_t n); from "../utils/factorial.h". */
#include <stdint.h>
#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Find the row r and column k such that
       r(r-1)/2 < n ≤ r(r+1)/2 */
    idx_t r = 1;
    idx_t cumulative = 1; /* cumulative count up to current row */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t prev_cumulative = cumulative - r;
    idx_t k = n - prev_cumulative;            /* 1‑based column index */

    /* t = r - k  (or equivalently n - k in triangle notation) */
    idx_t t = r - k;

    /* Compute a_t */
    seq_t a_t = 0;
    if (t % 4 == 0) {
        idx_t s = t / 4;
        if (s == 0) {
            a_t = -15;
        } else {
            /* cast to seq_t before multiplication to avoid overflow
               on intermediate 64‑bit integer types */
            a_t = -16 * (seq_t)(s + 1);
        }
    }
    if (a_t == 0) {
        return 0;
    }

    /* Compute the factorial ratio ((r-1)! / (k-1)!) */
    seq_t fac_r_minus1 = factorial(r - 1);
    seq_t fac_k_minus1 = factorial(k - 1);
    seq_t ratio = fac_r_minus1 / fac_k_minus1;

    /* Final value: (ratio * a_t) / 3 */
    seq_t result = (ratio * a_t) / 3;
    return result;
}
