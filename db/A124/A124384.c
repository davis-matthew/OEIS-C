/*
 * A124384  O.g.f.: A(x) = Sum_{n>=0} x^n*Product_{k=0..n} (1 + 2^k*x).
 * https://oeis.org/A124384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..⌊(n+1)/2⌋} [n-k+1 choose k]_2 * 2^{k(k-1)/2},
 *            where [m choose k]_2 is the Gaussian binomial coefficient with q
 *            = 2
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ------------------------------------------------------------
       Build a table of powers of two: pow2[i] = 2^i (exact,
       using the native seq_t type).  The table is extended only
       when a larger index is requested.  It is static so that
       successive calls can reuse the data.  */
    static seq_t *pow2 = NULL;   /* 2^i, i = 0.. */
    static size_t pow2_size = 0; /* number of entries allocated (>= needed) */

    /* We need powers up to exp = n+1 (included).                */
    size_t needed = (size_t)n + 2;   /* indices 0 … n+1 */

    if (pow2_size < needed) {
        /* Allocate new storage.  Preserve existing values.   */
        seq_t *tmp = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!tmp) return 0;  /* out‑of‑memory: simplest fallback */

        /* Copy old part (if any). */
        for (size_t i = 0; i < pow2_size; ++i) tmp[i] = pow2[i];

        /* Initialise the missing part.  For i = 0 we must set 1. */
        if (pow2_size == 0) {
            tmp[0] = (seq_t)1;
            pow2_size = 1;
        }

        /* Fill the rest by repeatedly doubling. */
        for (size_t i = pow2_size; i < needed; ++i)
            tmp[i] = tmp[i-1] << 1;   /* exact multiplication by 2 */

        /* Replace old table. */
        free(pow2);
        pow2 = tmp;
        pow2_size = needed;
    }

    /* ------------------------------------------------------------
       Compute the sum.  For each k we build the Gaussian binomial
       coefficient using the numerically stable order
       term *= (2^{m-k+i} - 1) / (2^i - 1),  i = 1 .. k.      */
    seq_t result = (seq_t)0;
    idx_t max_k = (n + 1) / 2;

    for (idx_t k = 0; k <= max_k; ++k) {
        seq_t term = (seq_t)1;          /* start with 1          */
        if (k > 0) {
            idx_t m = n - k + 1;        /* m ≥ k                  */
            for (idx_t i = 1; i <= k; ++i) {
                idx_t exp_num = m - k + i;   /* exponent for numerator   */
                idx_t exp_den = i;           /* exponent for denominator  */
                seq_t num = pow2[exp_num] - (seq_t)1;
                seq_t den = pow2[exp_den] - (seq_t)1;
                term *= num;
                term /= den;   /* exact division at each step   */
            }
        }
        /* Multiply by 2^{k(k-1)/2}.  For seq_t=uint64_t the shift
           is safe because for values that fit in 64‑bit this shift
           is < 64.  For arbitrary‑precision types the shift is
           supported by the class implementation.          */
        idx_t shift = (k * (k - 1)) / 2;
        if (shift > 0)
            term <<= shift;

        result += term;
    }

    return result;
}
