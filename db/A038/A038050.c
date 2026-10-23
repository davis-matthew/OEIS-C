/*
 * A038050  Number of labeled rooted trees with 3-colored leaves.
 * https://oeis.org/A038050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n, k)*2^k*(n-k)^(n-1))
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* NOTE:  n is 1‑based.  a(1)=3, a(2)=6, …  */

/* The implementation below uses only the utilities that are guaranteed
 * to be available (pow_int).  All intermediate values are performed
 * with the generic integer type seq_t, so the same code works for
 * native 8/16/32/64 and for arbitrary‑precision builds.  */
 
#include "../utils/pow_int.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;       /* should never be called, but safe */

    seq_t sum   = 0;
    seq_t binom = 1;            /* binomial(n,0) */
    seq_t pow2  = 1;            /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t  base = n - k;                          /* (n-k)  */
        seq_t  pw   = pow_int((seq_t)base, n - 1);    /* (n-k)^(n-1) */

        seq_t term = binom * pow2;   /* binomial * 2^k */
        term *= pw;                  /* * (n-k)^(n-1) */

        sum += term;

        if (k < n) {
            /* update binom to binomial(n, k+1) */
            binom = binom * (n - k) / (k + 1);
            /* pow2 *= 2;   shift left might not be supported for all seq_t types */
            pow2 = pow2 + pow2;    /* 2^k ** 2 = 2^(k+1) */
        }
    }
    return sum;
}
