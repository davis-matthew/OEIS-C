/*
 * A058615  McKay-Thompson series of class 30D for Monster.
 * https://oeis.org/A058615
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 43, u32 n <= 139, u64 n <= 478,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* This function returns the terms of OEIS sequence A058615.
   The sequence is defined by
       -2 + ((eta(q^2)*eta(q^3)*eta(q^10)*eta(q^15))/
              (eta(q)*eta(q^5)*eta(q^6)*eta(q^30)))^2
   where eta(q) = q^(1/24) ∏_{k≥1}(1 - q^k).
   The coefficients are taken from the expansion of the series above,
   with a(-1) being the coefficient of q^{-1}.  The implementation
   generates the product of the eta quotients by computing the
   exponents of each factor (1 - q^k) raised to an integer power,
   then expands the resulting infinite product up to the requested
   index.  Negative exponents are handled by a coin‑change routine
   (unbounded knapsack), while positive exponents are applied by
   repeated multiplication with (1 - q^k).  All arithmetic is
   performed on the type seq_t; values that exceed the range of
   seq_t wrap modulo 2^{width(seq_t)}.  The function keeps a static
   cache so that successive calls with larger indices can reuse
   previously computed data. */

seq_t a(idx_t n)
{
    if (n < -1)               /* indices below the offset are undefined */
        return (seq_t)0;

    /* Number of coefficients of H(q) = ∏(1 - q^k)^{2*e_k} that we must
       compute.  For a(-1) we need H[0]; for a(0) we need H[1];
       for a(n≥1) we need H[n+1].  Hence the required size is
       n+2 if n≥0, otherwise at least 1.  */
    size_t needed = (n >= 0 ? (size_t)(n + 2) : 1);

    /* Static cache.  h[i] holds the coefficient of q^i in H(q).   */
    static seq_t *h   = NULL;
    static size_t h_size = 0;

    /* Static array of exponents e_k = 2*(cnt_num - cnt_den) for
       1 ≤ k ≤ needed-1.  */
    static int *exps = NULL;
    static size_t exps_size = 0;

    if (needed > h_size) {
        /* Reallocate arrays to new size.  */
        free(h);
        free(exps);
        h = (seq_t*)malloc(needed * sizeof(seq_t));
        exps = (int*)malloc(needed * sizeof(int));
        if (!h || !exps) return (seq_t)0;   /* allocation error */

        /* Compute the exponent array e_k.  */
        const int num_set[4] = {2, 3, 10, 15};
        const int den_set[4] = {1, 5, 6, 30};

        for (size_t k = 1; k < needed; ++k) {
            int cnt_num = 0;
            for (int i = 0; i < 4; ++i)
                if (k % num_set[i] == 0) ++cnt_num;
            int cnt_den = 0;
            for (int i = 0; i < 4; ++i)
                if (k % den_set[i] == 0) ++cnt_den;
            exps[k] = 2 * (cnt_num - cnt_den);
        }

        /* Initialise the series H(q).  */
        h[0] = 1;
        for (size_t i = 1; i < needed; ++i) h[i] = 0;

        /* Expand the product.  */
        for (size_t k = 1; k < needed; ++k) {
            int e = exps[k];
            if (e == 0) continue;

            if (e > 0) {              /* multiply by (1 - q^k)^e */
                for (int iter = 0; iter < e; ++iter) {
                    /* scan descending to keep original values
                       for h[t-k] until they are updated. */
                    for (long long t = (long long)needed - 1; t >= (long long)k; --t) {
                        h[(size_t)t] -= h[(size_t)(t - k)];
                    }
                }
            } else {                   /* e < 0: multiply by (1 - q^k)^{-m} */
                int m = -e;
                for (int iter = 0; iter < m; ++iter) {
                    for (size_t t = k; t < needed; ++t) {
                        h[t] += h[t - k];
                    }
                }
            }
        }

        h_size = needed;
        exps_size = needed;
    }

    /* Return the requested term.  */
    if (n == -1)
        return h[0];          /* coefficient of q^0 in H(q) */
    if (n == 0) {
        seq_t res = h[1];
        res -= (seq_t)2;      /* subtract the constant -2 */
        return res;
    }
    /* n ≥ 1: coefficient of q^(n+1) in H(q) */
    return h[(size_t)n + 1];
}
