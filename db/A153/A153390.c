/*
 * A153390  G.f.: A(x) = F(x*G(x))^2 where F(x) = G(x*F(x)) = 1 + x*F(x)^3 is
 *          the g.f. of A001764 and G(x) = F(x/G(x)) = 1 + x*G(x)^2 is the
 *          g.f. of A000108 (Catalan).
 * https://oeis.org/A153390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=0..n} C(3k+2,k)*2/(3k+2) * C(2n-k,n-k)*k/(2n-k),
 *            a(0)=1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 9, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The computation uses only integer arithmetic; all divisions are exact. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t sum = (seq_t)0;

    /*  V(k) = C(2n-k, n-k).  Start with V(0) = C(2n, n). */
    seq_t V_prev = binom(2 * n, n);
    /*  T(k) = C(3k+2,k) * 2 / (3k+2).  Start with T(0)=1. */
    seq_t T_curr = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        /* Update V(k) from V(k-1) */
        idx_t k_minus_1 = k - 1;
        seq_t V_k = (V_prev * (seq_t)(n - k_minus_1)) / (seq_t)(2 * n - k_minus_1);

        /* Compute U(k) = V(k) * k / (2n - k) */
        seq_t U_k = (V_k * (seq_t)k) / (seq_t)(2 * n - k);

        /* Compute T(k) from T(k-1) using the recurrence:
           T(k) = T(k-1) * (3(k-1)+4)(3(k-1)+3)(3(k-1)+2) / ((k)(2k+4)(2k+3)) */
        uint64_t num = (uint64_t)(3 * k_minus_1 + 4) *
                       (3 * k_minus_1 + 3) *
                       (3 * k_minus_1 + 2);
        uint64_t den = (uint64_t)(k) *
                       (2 * k_minus_1 + 4) *
                       (2 * k_minus_1 + 3);
        seq_t T_k = (T_curr * (seq_t)num) / (seq_t)den;

        /* Add term to sum */
        sum += T_k * U_k;

        /* Prepare for next iteration */
        V_prev = V_k;
        T_curr = T_k;
    }

    return sum;
}
