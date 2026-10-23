/*
 * A173610  Bisections are {b(n)*b(n+1), n>=0} (shifted) and {b(n)^2, n>=0}
 *          where {b(n), n>=0} is the self-convolution of this sequence, with
 *          a(0)=1.
 * https://oeis.org/A173610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0) = 1.
   Define b(m) = Σ_{i=0}^{m} a(i)*a(m-i)   (self‑convolution of a).
   Then for k ≥ 0:
        a(2k+1) = b(k)^2
        a(2k)   = b(k-1)*b(k)   (k ≥ 1)
   with b(0) = a(0)^2 = 1.
   The function computes a(n) iteratively using the above relations. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* allocate arrays for a[0..n] and b[0..⌊n/2⌋] */
    size_t anum = (size_t)n + 1;
    size_t bnum = (size_t)(n / 2) + 1;   /* ceiling of n/2 + 1, safe for n=0 */

    seq_t *aarr = (seq_t *)malloc(anum * sizeof(seq_t));
    seq_t *barr = (seq_t *)malloc(bnum * sizeof(seq_t));
    if (!aarr || !barr) {
        /* memory allocation failure: return 0 as safe fallback */
        return (seq_t)0;
    }

    aarr[0] = (seq_t)1;
    barr[0] = (seq_t)1;  /* b(0) = a(0)*a(0) = 1 */

    seq_t val = (seq_t)0;  /* value to be returned at the end */

    for (idx_t m = 1; m <= n; ++m) {
        if (m & 1) { /* m is odd: m = 2k+1 */
            idx_t k = (m - 1) >> 1;
            aarr[m] = barr[k] * barr[k];
        } else { /* m is even: m = 2k */
            idx_t k = m >> 1;
            /* compute b(k) = Σ_{i=0}^{k} a(i)*a(k-i) */
            seq_t sum = (seq_t)0;
            for (idx_t i = 0; i <= k; ++i) {
                sum += aarr[i] * aarr[k - i];
            }
            barr[k] = sum;
            /* a(2k) = b(k-1) * b(k) for k >= 1 */
            aarr[m] = barr[k - 1] * barr[k];
        }
        if (m == n) {
            val = aarr[m];
        }
    }

    free(aarr);
    free(barr);
    return val;
}
