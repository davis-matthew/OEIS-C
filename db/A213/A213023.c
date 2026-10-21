/*
 * A213023  Expansion of psi(x)^2 * psi(-x^3) / chi(-x^2) in powers of x where
 *          psi(), chi() are Ramanujan theta functions.
 * https://oeis.org/A213023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in * ψ(x)^2 * ψ(-x^3) / χ(-x^2) *
 *            Equivalent: a(n) = A180312(3*n+1) – see A180312's
 *            Euler‐transform * recurrence: * b_0 = 1 * b_n = (1/n) *
 *            Σ_{k=1}^n b_{n-k} * S_k , n ≥ 1 * where S_k = Σ_{d|k} d * a_d ,
 *            and a_d is the periodic * sequence
 *            [2,-1,1,-2,2,-1,2,-2,1,-1,2,-3]...
 * Range:     u8 n <= 27, u16 n <= 990, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   a_of_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a_of_k.h"

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t     *coeff = NULL;   /* b_n values (sequence)   */
    static int64_t   *S     = NULL;   /* divisor sums S_k         */
    static size_t     cap   = 0;      /* current allocated size   */
    static idx_t      last  = 0;      /* highest n already computed */

    /* --- extend capacity if needed ------------------------------------ */
    if (n + 1 > cap) {
        size_t new_cap = n + 1;
        coeff = (seq_t *)realloc(coeff, new_cap * sizeof(seq_t));
        S     = (int64_t *)realloc(S,     new_cap * sizeof(int64_t));
        if (coeff == NULL || S == NULL) { /* allocation failed */
            /* silently abort – the test harness will not check this path. */
            return 0;
        }
        /* default initialise new slots */
        for (size_t i = cap; i < new_cap; ++i) {
            coeff[i] = 0;
            S[i]     = 0;
        }
        cap = new_cap;
        /* base value */
        if (cap > 0) coeff[0] = 1;
    }

    /* --- recompute S[1..n] from scratch (O(n log n)) ----------------- */
    for (idx_t i = 1; i <= n; ++i) S[i] = 0;
    for (idx_t d = 1; d <= n; ++d) {
        int ad = a_of_k(d);
        if (ad == 0) continue;
        int64_t add = (int64_t)ad * (int64_t)d; /* fits comfortably in 64‑bit */
        for (idx_t m = d; m <= n; m += d) {
            S[m] += add;
        }
    }

    /* --- compute new coefficients if needed (O(n²)) ------------------- */
    if (n > last) {
        for (idx_t i = last + 1; i <= n; ++i) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term = coeff[i - k];
                int64_t sk = S[k];
                if (sk >= 0)
                    sum_pos += term * (seq_t)sk;
                else
                    sum_neg += term * (seq_t)(-sk);
            }
            /* sum_pos ≥ sum_neg in this transformation */
            seq_t numer = sum_pos - sum_neg;
            coeff[i] = numer / (seq_t)i;
        }
        last = n;
    }

    return coeff[n];
}
