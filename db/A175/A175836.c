/*
 * A175836  a(n) = Product_{i=1..n} psi(i) where psi is the Dedekind psi
 *          function (A001615).
 * https://oeis.org/A175836
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A175836:  a(n) = ∏_{i=1..n} ψ(i)   where ψ(i) = i * ∏_{p∣i}(1+1/p)  */
/*  ψ(i) is the Dedekind psi function (A001615).  */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_spf.h"

/* External linkage as requested */
seq_t a(idx_t n)
{
    /*  The sequence is defined for n ≥ 1.  For n < 1 we return 0. */
    if (n < 1) return 0;

    /*  Static cache for smallest‑prime‑factor array.  */
    static idx_t *spf = NULL;            /* spf[k] = smallest prime divisor of k */
    static idx_t spf_limit = 0;          /* highest number for which spf is valid */

    /*  Extend the spf cache if necessary.  */
    if (n > spf_limit) {
        if (spf != NULL) free(spf);      /* free old table if it exists */
        build_spf(n, &spf);              /* build table up to n  */
        spf_limit = n;
    }

    seq_t prod = 1;                      /* cumulative product ∏ψ(i) */

    for (idx_t i = 1; i <= n; ++i) {
        /* compute ψ(i) */
        seq_t psi = (seq_t)i;
        idx_t temp = i;

        while (temp > 1) {
            idx_t p = spf[temp];
            if (p == 0) p = temp;       /* safety if spf[1] is 0 */

            psi = psi * (seq_t)(p + 1);
            psi = psi / (seq_t)p;

            while (temp % p == 0) temp /= p;
        }

        prod = prod * psi;
    }

    return prod;
}
