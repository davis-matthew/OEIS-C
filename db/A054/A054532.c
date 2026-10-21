/*
 * A054532  Ramanujan sum T(n, k) = c_k(n) = Sum_{m=1..k, (m,k)=1}
 *          exp(2*Pi*i*m*n / k), triangular array read by rows for n >= 1 and
 *          1 <= k <= n.
 * https://oeis.org/A054532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy, nice, tabl
 * Formula:   a(n) = Ramanujan sum c_k(row) where row, k are obtained from the
 *            flattened index n by reading the triangular array row by row.
 *            c_k(r) = ∑_{d∣gcd(k,r)} μ(k/d)·d . This implementation uses the
 *            mu_of(idx_t d) helper
 * Range:     i8 n <= 1275, i16 n <= 1275, i32 n <= 1275, i64 n <= 1275,
 *            i128 n <= 1275, big any n
 * Helpers:   gcd_uint64, mu_of, unravel
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/mu_of.h"
#include "../utils/gcd_uint64.h"
#include "../utils/unravel.h"

/* Compute Ramanujan sum T(r,k) (with r≥1, 1≤k≤r) using the divisor‑sum formula */
static seq_t ramanujan_c(idx_t r, idx_t k)
{
    seq_t sum = 0;
    uint64_t g   = gcd_uint64((uint64_t)k, (uint64_t)r);
    for (idx_t d = 1; d * d <= g; ++d) {
        if (g % d == 0) {
            int mu1 = mu_of(k / d);
            sum += (seq_t)mu1 * (seq_t)d;
            idx_t d2 = g / d;
            if (d2 != d) {
                int mu2 = mu_of(k / d2);
                sum += (seq_t)mu2 * (seq_t)d2;
            }
        }
    }
    return sum;
}

/* Required sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not defined, but guard */
    idx_t r, k;
    unravel(n, &r, &k);
    return ramanujan_c(r, k);
}
