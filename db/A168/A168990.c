/*
 * A168990  Number of reduced words of length n in Coxeter group on 25
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^23 = I.
 * https://oeis.org/A168990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A168990:
   Growth series generating function:
     G(t) = (1 + 2t + 2t^2 + ... + 2t^22 + t^23)
            ------------------------------------
              1 - 23t - 23t^2 - ... - 23t^22 + 276t^23
   The corresponding linear recurrence (valid for all n ≥ 0) is:
      a(n) = b(n) + 23 * Σ_{k=1}^{min(22,n)} a(n-k) - 276 * a(n-23),
   where b(0)=1, b(1…22)=2, b(23)=1, and b(n)=0 for n>23.
   Initial value a(0)=1.
   This recurrence yields all terms of the sequence. */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)1;

    seq_t *a = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
    if (!a)
        return (seq_t)0; /* allocation failure */

    a[0] = (seq_t)1;

    for (idx_t i = (idx_t)1; i <= n; ++i) {
        /* sum of the previous up to 22 terms */
        seq_t sum = 0;
        idx_t kmax = i < 22 ? i : 22;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += a[i - k];

        /* start building a[i] */
        seq_t val = sum * (seq_t)23;          /* 23 * sum previous terms */

        /* add b(i) */
        if (i <= 22)
            val += 2;                         /* b(i)=2 for i=1…22 */
        else if (i == 23)
            val += 1;                         /* b(23)=1 */
        /* for i>23, b(i)=0, nothing to add */

        /* subtract 276 * a[i-23] if applicable */
        if (i >= 23)
            val -= a[i - 23] * (seq_t)276;

        a[i] = val;
    }

    seq_t result = a[n];
    free(a);
    return result;
}
