/*
 * A055453  a(n) = T(n,n-3), array T as in A055450.
 * https://oeis.org/A055453
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 19, u64 n <= 33, u128 n <= 61,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A055453: a(n) = T(n,n-3)
   Computed from the hypergeometric representation
   a(n) = 14 * ₂F₁([6-n, 9/2], [6], -4)  for n ≥ 6
   a(n) = (n-2)²            for 3 ≤ n ≤ 5
   The implementation uses the finite sum
        a(n) = Σ_{k=0}^{n-6} binomial(n-6,k) * E_k
   where
        E_0 = 14
        E_k = E_{k-1} * (2k+9) * 2 / (k+6)     (k ≥ 0, 1‑based,
        i.e. E_{k+1} = E_k * (2(k+1)+7) * 2 / (k+1+5) ).
   All arithmetic is performed with exact integer division
   using gcd to cancel factors before multiplication, so that
   the routine works with any unsigned seq_t type (including
   arbitrary‑precision integers) without overflow when the
   result fits in seq_t.  */ 

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Offset handling – return (n-2)² for n=3..5 */
    if (n <= 5)
    {
        seq_t v = (seq_t)(n - 2);
        return v * v;
    }

    /* n ≥ 6 */
    idx_t m = n - 6;            /* number of summation terms: k=0..m */
    seq_t sum = 0;              /* running total of binomial * E */
    seq_t bin = 1;              /* binomial(m,0) */
    seq_t E   = 14;             /* E₀ */
    sum += bin * E;             /* term for k = 0 */

    for (idx_t k = 0; k < m; ++k)
    {
        /* -------- update binomial coefficient bin = binom(m,k+1) -------- */
        idx_t num_n = m - k;            /* numerator part (m - k) */
        idx_t den_d = k + 1;            /* denominator part (k + 1) */

        seq_t g = gcd((seq_t)bin, (seq_t)den_d);
        seq_t bin1 = bin / g;
        seq_t den1 = den_d / g;

        g = gcd((seq_t)num_n, den1);
        seq_t num1 = num_n / g;
        den1 /= g;

        seq_t new_bin = bin1 * num1;     /* division is exact */

        /* -------- update E : E_{k+1} = E_k * (2(k+1)+7) * 2 / (k+1+5) -------- */
        idx_t kp1 = k + 1;
        seq_t num_a = (seq_t)(2 * kp1 + 7);   /* (2k+9)   */
        seq_t num_b = 2;                     /* factor 2   */
        seq_t den_c = (seq_t)(kp1 + 5);       /* (k+6)    */

        /* cancel with E */
        g = gcd((seq_t)E, den_c);
        seq_t E1 = E / g;
        den_c /= g;

        /* cancel with num_a */
        g = gcd(num_a, den_c);
        seq_t a1 = num_a / g;
        den_c /= g;

        /* cancel with num_b */
        g = gcd(num_b, den_c);
        seq_t b1 = num_b / g;
        den_c /= g;

        /* multiply remaining factors – division by den_c should be exact */
        seq_t tmp = E1 * a1;
        if (den_c != 1)
            tmp /= den_c;                 /* exact division */
        tmp *= b1;                         /* multiply by remaining 2 if any */

        seq_t new_E = tmp;

        /* apply updated values */
        bin = new_bin;
        E   = new_E;

        /* add current term to the sum */
        sum += bin * E;
    }

    return sum;
}
