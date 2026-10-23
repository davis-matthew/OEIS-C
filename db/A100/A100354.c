/*
 * A100354  Maximal number of 1432 patterns in a permutation of 1,2,...,n.
 * https://oeis.org/A100354
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = max_{1<=k<n} ( a(k) + k*choose(n-k,3) ) where choose(t,3)
 *            = t*(t-1)*(t-2)/6 (0 if t<3)
 * Range:     u8 n <= 12, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   choose3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=0 */

#include <stdlib.h>
#include "../utils/choose3.h"

static seq_t *A = NULL;     /* memoized array of previous values */
static idx_t  A_size = 0;   /* allocated size (highest index+1) */

seq_t a(idx_t n)
{
    /* ensure array big enough to hold indices up to n */
    if (A_size == 0) {
        A = (seq_t *)malloc((size_t)2 * sizeof(seq_t)); /* indices 0 and 1 used */
        if (!A) return (seq_t)0;     /* allocation failure – return 0 */
        A[1] = (seq_t)0;             /* a(1) = 0 */
        A_size = 2;
    }

    if (n + 1 > A_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(A, new_size * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure – return 0 */
        A = tmp;
        /* compute new entries from old size to n */
        for (idx_t i = A_size; i <= n; ++i) {
            if (i == 1) {
                A[1] = (seq_t)0;
                continue;
            }
            seq_t best = (seq_t)0;
            for (idx_t k = 1; k < i; ++k) {
                seq_t t = (seq_t)(i - k);                     /* n-k */
                seq_t term = A[k] + (k * t * (t - 1) * (t - 2)) / 6;
                if (term > best)
                    best = term;
            }
            A[i] = best;
        }
        A_size = new_size;
    }

    return A[n];
}
