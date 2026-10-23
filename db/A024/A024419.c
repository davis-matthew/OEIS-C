/*
 * A024419  a(n) = n! (1/C(n,0) + 1/C(n,1) + ... + 1/C(n,[ n/2 ])).
 * https://oeis.org/A024419
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} factorial(k) * factorial(n-k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) for any non‑negative n using O(n) time and
   memoized factorials. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached factorials: fac[i] = i! */
    static seq_t *fac = NULL;        /* pointer to factorial array */
    static idx_t fac_cap = 0;        /* number of elements allocated */

    /* First call: allocate space for factorial(0) */
    if (fac == NULL) {
        fac_cap = 1;
        fac = (seq_t *)malloc(fac_cap * sizeof(seq_t));
        if (!fac) return (seq_t)0;          /* allocation failed */
        fac[0] = (seq_t)1;                  /* 0! = 1 */
    }

    /* Grow factorial array if necessary */
    if ((idx_t)n >= fac_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(fac, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* allocation failed */

        /* Compute missing factorials */
        for (idx_t i = fac_cap; i <= n; ++i) {
            tmp[i] = tmp[i - 1] * (seq_t)i;
        }
        fac = tmp;
        fac_cap = new_cap;
    }

    /* Sum the half‑convolution of factorials */
    seq_t sum = (seq_t)0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k) {
        sum += fac[k] * fac[n - k];
    }
    return sum;
}
