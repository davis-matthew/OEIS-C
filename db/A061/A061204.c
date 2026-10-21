/*
 * A061204  (tau<=)_6(n).
 * https://oeis.org/A061204
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=1..n} τ₅(k) * ⌊n/k⌋
 * Range:     u8 n <= 11, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   binom, build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  τ₅(k) = number of ordered 5-tuples of positive integers with product ≤ k
    (equivalently the number of multiplicative compositions of k into 5 parts)
    and can be computed from the prime factorisation of k as
    τ₅(k) = Π_{p^e‖k} C(e+4,4). */

#include <stdlib.h>
#include "../utils/build_spf.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Static state reused between calls. */
    static idx_t   max_n  = 0;          /* largest n for which tables are built */
    static idx_t * spf    = NULL;      /* smallest prime factor array [0..max_n] */
    static seq_t * tau5   = NULL;      /* τ₅(k) for k ≤ max_n, index 0 unused */

    /* Rebuild tables if requested n exceeds current capacity. */
    if (n > max_n) {
        /* Rebuild the smallest‑prime‑factor (spf) table for the new limit. */
        /* build_spf is designed to free an existing array, so we pass the
           current pointer (may be NULL). */
        build_spf(n, &spf);

        /* Allocate a new τ₅ array of the required size. */
        if (tau5) free(tau5);
        tau5 = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!tau5) return 0;          /* allocation failure – unlikely for n ≤ 10000 */

        tau5[0] = 0;                  /* unused entry */

        /* Compute τ₅(k) for every k ≤ n. */
        for (idx_t k = 1; k <= n; ++k) {
            idx_t   x   = k;
            seq_t   tau = 1;

            while (x > 1) {
                idx_t p = spf[x];
                idx_t e = 0;
                while (x % p == 0) {
                    x /= p;
                    ++e;
                }
                /* Multiply by the binomial coefficient C(e+4,4). */
                tau *= binom(e + 4, 4);
            }
            tau5[k] = tau;
        }
        max_n = n;
    }

    /* Compute a(n) = Σ τ₅(k) * ⌊n/k⌋. */
    seq_t result = 0;
    for (idx_t k = 1; k <= n; ++k) {
        result += tau5[k] * (seq_t)(n / k);
    }
    return result;
}
