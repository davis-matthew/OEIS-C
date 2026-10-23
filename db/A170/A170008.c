/*
 * A170008  Number of reduced words of length n in Coxeter group on 47
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^35 = I.
 * https://oeis.org/A170008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n) + 45·∑_{i=1}^{34} a(n−i) − 1035·a(n−35) where c(n)=1
 *            for n=0 or 35, c(n)=2 for 1≤n≤34, and c(n)=0 otherwise. This
 *            follows from the rational generating function (t^35 + 2t^34 + …
 *            + 2t + 1) / (1 − 45t − 45t^2 − … − 45t^34 + 1035t^35)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Computes the n‑th term of OEIS A170008.  All arithmetic is performed
   in the unsigned integer type seq_t supplied by the build.  The routine
   uses only a small fixed buffer of 36 elements, so it works for any
   build (C or C++) without dynamic allocation or re‑allocation.  */
extern seq_t a(idx_t n)
{
    /* small circular buffer holding the last 35 computed values
       (and a0).  Index k corresponds to ring[ k % 36 ].  */
    static seq_t ring[36];
    /* the buffer is zero‑initialized on the first call   */
    static int initialized = 0;
    if (!initialized) {
        for (size_t i = 0; i < 36; ++i) ring[i] = 0;
        ring[0] = 1;          /* a(0) = 1  */
        initialized = 1;
    }

    if (n == 0) return ring[0];  /* return 1 */

    /* compute a(k) for k=1..n in order  */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t c = 0;
        if (k == 35)               c = 1;
        else if (k <= 34)          c = 2;   /* 1 ≤ k ≤ 34 */

        seq_t sum = 0;
        /* add 45·a(k-i) for i = 1..34 if k-i ≥ 0  */
        for (idx_t i = 1; i <= 34; ++i) {
            if (k >= i) {
                sum += ring[(k - i) % 36] * 45;
            }
        }
        /* subtract 1035·a(k-35) if k ≥ 35  */
        if (k >= 35) {
            sum -= ring[(k - 35) % 36] * 1035;
        }

        /* a(k) = c + sum  */
        seq_t ak = c + sum;
        ring[k % 36] = ak;
    }

    return ring[n % 36];
}
