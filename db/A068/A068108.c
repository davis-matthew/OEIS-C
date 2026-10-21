/*
 * A068108  a(1) = 1; a(n+1) = sum{k|n k<=sqrt(n)} a(k) where sum is over the
 *          positive divisors k of n with k <= sqrt(n).
 * https://oeis.org/A068108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; for n ≥ 1,
   a(n+1) = Σ_{d|n , d ≤ √n} a(d)  (sum over divisors d of n not exceeding √n) */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static memoisation array (index 1‑based) */
    static seq_t *A = NULL;
    static size_t cap = 0;           /* current allocated size (max index +1) */
    static idx_t  computed = 0;      /* highest index already computed */

    if (n < 1)
        return 0;                    /* undefined but never called with n==0 */

    /* If array too small, grow it.  Ensure we have at least n+1 entries. */
    if ((size_t)n >= cap) {
        size_t new_cap = cap ? cap << 1 : 64;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                 /* allocation failure */
        /* initialise newly allocated portion to 0 (not strictly needed) */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = 0;
        A = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n. */
    if (n > computed) {
        /* initialise the very first term if needed */
        if (computed == 0) {
            A[1] = 1;
            computed = 1;
        }

        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t sum = 0;
            idx_t m = i - 1;                /* divisors of (i-1) are needed */
            for (idx_t d = 1; d * d <= m; ++d) {
                if (m % d == 0)
                    sum += A[d];            /* d ≤ √m by loop condition */
            }
            A[i] = sum;
        }
        computed = n;
    }

    return A[n];
}
